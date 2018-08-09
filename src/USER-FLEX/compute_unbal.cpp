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

#include <mpi.h>
#include "compute_unbal.h"
#include "atom.h"
#include "update.h"
#include "force.h"
#include "domain.h"
#include "group.h"
#include "error.h"

using namespace LAMMPS_NS;

/* ---------------------------------------------------------------------- */

ComputeUnbal::ComputeUnbal(LAMMPS *lmp, int narg, char **arg) :
  Compute(lmp, narg, arg)
{
  if (narg != 3) error->all(FLERR,"Illegal compute unbal command");

  scalar_flag = 1;
}

/* ---------------------------------------------------------------------- */

void ComputeUnbal::init()
{
  unbal_max = 0.0;
}

/* ---------------------------------------------------------------------- */

double ComputeUnbal::compute_scalar()
{
  double unbal = 0.0;
  double **f = atom->f;
  int *mask = atom->mask;
  int *type = atom->type;
  int nlocal = atom->nlocal;

  unbal_max = 0.0;

  for (int i = 0; i < nlocal; i++)
    if (mask[i] & groupbit) {
      unbal = sqrt( f[i][0]*f[i][0] + f[i][1]*f[i][1] + f[i][2]*f[i][2] );

      if( unbal > unbal_max )  unbal_max = unbal ;
    } 

  MPI_Allreduce(&unbal_max,&scalar,1,MPI_DOUBLE,MPI_MAX,world);
  return scalar;
}
