/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#include <cmath>
#include <cstdlib>
#include <cstring>
#include "fix_damp_local.h"
#include "atom.h"
#include "update.h"
#include "respa.h"
#include "domain.h"
#include "error.h"
#include "force.h"

using namespace LAMMPS_NS;
using namespace FixConst;

/* ---------------------------------------------------------------------- */

FixDampLocal::FixDampLocal(LAMMPS *lmp, int narg, char **arg) :
  Fix(lmp, narg, arg)
{
  if (narg != 4) error->all(FLERR,"Illegal fix damp/local command");

  dynamic_group_allow = 1;
  scalar_flag = 1;
  global_freq = 1;
  extscalar = 1;
  respa_level_support = 1;
  ilevel_respa = 0;

  alpha = force->numeric(FLERR,arg[3]);

  fdamplocal[0] = fdamplocal[1] = fdamplocal[2] = 0.0;
}

/* ---------------------------------------------------------------------- */

int FixDampLocal::setmask()
{
  int mask = 0;
  mask |= POST_FORCE;
  mask |= POST_FORCE_RESPA;
  return mask;
}

/* ---------------------------------------------------------------------- */

void FixDampLocal::init()
{
  if (strstr(update->integrate_style,"respa")) {
    ilevel_respa = ((Respa *) update->integrate)->nlevels-1;
    if (respa_level >= 0) ilevel_respa = MIN(respa_level,ilevel_respa);
  }
}

/* ---------------------------------------------------------------------- */

void FixDampLocal::setup(int vflag)
{
  if (strstr(update->integrate_style,"verlet"))
    post_force(vflag);
  else {
    ((Respa *) update->integrate)->copy_flevel_f(ilevel_respa);
    post_force_respa(vflag,ilevel_respa,0);
    ((Respa *) update->integrate)->copy_f_flevel(ilevel_respa);
  }
}

/* ---------------------------------------------------------------------- */

void FixDampLocal::post_force(int vflag)
{
  // apply drag force to atoms in group of magnitude f_mag
  // apply in direction (r-r0) if atom is further than delta away

  double **v = atom->v;
  double **f = atom->f;
  int *mask = atom->mask;
  int nlocal = atom->nlocal;

  force_flag = 0;

  for (int i = 0; i < nlocal; i++)
    if (mask[i] & groupbit) {
      if( (v[i][0] * f[i][0]) > 0 )
       fdamplocal[0] = - alpha * f[i][0];
      else
       fdamplocal[0] = alpha * f[i][0];
      if( (v[i][1] * f[i][1]) > 0 )
       fdamplocal[1] = - alpha * f[i][1];
      else
       fdamplocal[1] = alpha * f[i][1];
      if( (v[i][2] * f[i][2]) > 0 )
       fdamplocal[2] = - alpha * f[i][2];
      else
       fdamplocal[2] = alpha * f[i][2];
      f[i][0] += fdamplocal[0];
      f[i][1] += fdamplocal[1];
      f[i][2] += fdamplocal[2];
      }

  fdamplocalabs = sqrt( fdamplocal[0]*fdamplocal[0] + fdamplocal[1]*fdamplocal[1] + fdamplocal[2]*fdamplocal[2] ) ;
}

/* ---------------------------------------------------------------------- */

void FixDampLocal::post_force_respa(int vflag, int ilevel, int iloop)
{
  if (ilevel == ilevel_respa) post_force(vflag);
}

/* ----------------------------------------------------------------------
   return components of total drag force on fix group
------------------------------------------------------------------------- */

double FixDampLocal::compute_scalar()
{
  // only sum across procs one time

  if (force_flag == 0) {
    MPI_Allreduce(&fdamplocalabs,&fdamplocalabs_max,1,MPI_DOUBLE,MPI_MAX,world);
    force_flag = 1;
  }
  return fdamplocalabs_max;
}

