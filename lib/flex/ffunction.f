      Function ff( a , b )

       Use iso_c_binding

       Real( c_double )                :: ff
       Real( c_double ) , Dimension(*) :: a
       Real( c_double ) , Dimension(*) :: b
 
       Interface
        Function f( a , b ) bind(c)
         Use iso_c_binding
         Real( c_double )                :: f
         Real( c_double ) , Dimension(*) :: a
         Real( c_double ) , Dimension(*) :: b
        End Function
       End Interface

       ff = f( a , b )

       Write(*,*) "ff=" , ff

      End Function

