      Program P

       Interface
        Function ff( a , b )
         Real( kind=8 )                :: ff
         Real( kind=8 ) , Dimension(*) :: a
         Real( kind=8 ) , Dimension(*) :: b
        End Function
       End Interface

       Real( kind=8 ) , Dimension(6) :: a
       Real( kind=8 ) , Dimension(6) :: b
       Real( kind=8 )                :: c = 0

       a = (/ 1.0 , 2.0 , 3.0 , 4.0 , 5.0 , 6.0 /)
       b = (/ 2.0 , 3.0 , 4.0 , 5.0 , 6.0 , 7.0 /)

       c = ff( a , b )

       Write(*,*) 'c=' , c

      End Program

