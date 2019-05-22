integer i,j,atom_ida,atom_style,mol_style,idum1,idum2
real:: act_x,act_y,z,q,mux,muy,muz,ra1,ra2,ran2,pi=3.14

open(1,file='active.dat') 
open(2,file='wall.dat')


 
!active particles

atom_ida=0

do i=-90,-41
do j=-50,49
    atom_ida=atom_ida+1
    atom_style=1
    act_x=i
    act_y=j
    z=0
    mol_style=mol_style+1
    q=1
    idum1=12345
    idum2=54321
    ra1=ran2(idum1)
    ra2=ran2(idum2)
    mux=2.0*ra1-1.0
    muy=2.0*ra2-1.0
    muz=0
    write(1,*)atom_ida,atom_style,act_x,act_y,z,mol_style,q,mux,muy,muz  
end do
end do

do i=60,64
do j=-12,12
    atom_ida=atom_ida+1
    atom_style=1
    act_x=i
    act_y=j
    z=0
    mol_style=mol_style+1
    q=1
    idum1=12345
    idum2=54321
    ra1=ran2(idum1)
    ra2=ran2(idum2)
    mux=2.0*ra1-1.0
    muy=2.0*ra2-1.0
    muz=0
    write(1,*)atom_ida,atom_style,act_x,act_y,z,mol_style,q,mux,muy,muz  
end do
end do


!wall
atom_idw=atom_ida
r=35
o=2*pi/1099
do i=1,1099
   wall_x=r*cos(o*i)
   wall_y=r*sin(o*i)
   wall_z=0
   atom_idw=atom_idw+1
   atom_style=2
   mol_style=mol_style+1
   q=1
   mux=0
   muy=0
   muz=0
    write(2,*) atom_idw,atom_style,wall_x,wall_y,wall_z,mol_style,q,mux,muy,muz   
end do   
   

close(1)
close(2)

end 
 
 
 FUNCTION ran2(idum)
      INTEGER idum,IM1,IM2,IMM1,IA1,IA2,IQ1,IQ2,IR1,IR2,NTAB,NDIV
      REAL ran2,AM,EPS,RNMX
      PARAMETER (IM1=2147483563,IM2=2147483399,AM=1./IM1,IMM1=IM1-1,&
      IA1=40014,IA2=40692,IQ1=53668,IQ2=52774,IR1=12211,IR2=3791,&
      NTAB=32,NDIV=1+IMM1/NTAB,EPS=1.2e-7,RNMX=1.-EPS)
      INTEGER idum2,j,k,iv(NTAB),iy
      SAVE iv,iy,idum2
      DATA idum2/123456789/, iv/NTAB*0/, iy/0/
      if (idum.le.0) then
        idum=max(-idum,1)
        idum2=idum
        do 20 j=NTAB+8,1,-1

          k=idum/IQ1
          idum=IA1*(idum-k*IQ1)-k*IR1
          if (idum.lt.0) idum=idum+IM1
          if (j.le.NTAB) iv(j)=idum
20      continue
        iy=iv(1)
      endif
      k=idum/IQ1
      idum=IA1*(idum-k*IQ1)-k*IR1
      if (idum.lt.0) idum=idum+IM1
      k=idum2/IQ2
      idum2=IA2*(idum2-k*IQ2)-k*IR2
      if (idum2.lt.0) idum2=idum2+IM2
      j=1+iy/NDIV
      iy=iv(j)-idum2
      iv(j)=idum
      if(iy.lt.1)iy=iy+IMM1
      ran2=min(AM*iy,RNMX)
      return
      END
    