/* 1-D finite-difference wave extrapolation */
/*
  Copyright (C) 2008 University of Texas at Austin
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <rsf.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif
int main(int argc, char* argv[]) 
{
    int nx, nz, nt, ix, iz, it, sl, what, nb1, nbx, nb2, nbz;
    float dt, dx, dz, w1, w2, h1, h2, a1, a2, a3, a4, lmd=0.015;
    float **old, **nxt, **cur, *sig, **v, **vx, **vz, **a, **b10, **b20, **b01, **b02; 
    sf_file inp, out, vel, grad1, grad2;
    #ifdef _OPENMP
    int nth;
    #endif

    sf_init(argc,argv);
    inp = sf_input("in");
    out = sf_output("out");
    vel = sf_input("vel");   /* velocity */
    grad1 = sf_input("grad1"); /* velocity gradient gx */
    grad2 = sf_input("grad2"); /* velocity gradient gz*/

    if (SF_FLOAT != sf_gettype(inp)) sf_error("Need float input");
    if (!sf_histint(vel,"n1",&nx)) sf_error("No n1= in input");
    if (!sf_histfloat(vel,"d1",&dx)) sf_error("No d1= in input");
    if (!sf_histint(vel,"n2",&nz)) sf_error("No n2= in input");
    if (!sf_histfloat(vel,"d2",&dz)) sf_error("No d2= in input");
    if (!sf_histint(inp,"n1",&nt)) sf_error("No n1= in input");
    if (!sf_histfloat(inp,"d1",&dt)) sf_error("No d1= in input");


    sf_putint(out,"n1",nx);
    sf_putfloat(out,"d1",dx);
    sf_putint(out,"n2",nz);
    sf_putfloat(out,"d2",dz);
    sf_putint(out,"n3",nt);
    sf_putfloat(out,"d3",dt);

    if (!sf_getint("sl",&sl)) sl=nx/2; /* source location*/
    if (!sf_getint("what",&what)) what=2; /* 2nd or 4th order for FD*/
    if (!sf_getint("nb1",&nb1)) nb1=20; /* x boundary nodes */
    if (!sf_getint("nb2",&nb2)) nb2=20; /* z boundary nodes*/

    nbx = nb1*2 + nx;
    nbz = nb2 + nz;
    sig = sf_floatalloc(nt);
    old = sf_floatalloc2(nbx,nbz);
    nxt = sf_floatalloc2(nbx,nbz);
    cur = sf_floatalloc2(nbx,nbz);
    v = sf_floatalloc2(nbx,nbz);
    vx = sf_floatalloc2(nbx,nbz);
    vz = sf_floatalloc2(nbx,nbz);
  
    for (iz=0; iz < nz; iz++) {
        sf_floatread(v[iz],nx,vel);
        sf_floatread(vx[iz],nx,grad1);
        sf_floatread(vz[iz],nx,grad2);
    
        for (ix= nx+nb1-1; ix > nb1-1; ix--) {
            v[iz][ix] = v[iz][ix-nb1];
            vx[iz][ix] = vx[iz][ix-nb1];
            vz[iz][ix] = vz[iz][ix-nb1];
         } 
    
    for (ix=0; ix < nb1; ix++){
        v[iz][ix] = v[iz][nb1];
        vx[iz][ix] = vx[iz][nb1];
        vz[iz][ix] = vz[iz][nb1];
        v[iz][ix+nx+nb1] = v[iz][nx+nb1-1];
        vx[iz][ix+nx+nb1] = vx[iz][nx+nb1-1];
        vz[iz][ix+nx+nb1] = vz[iz][nx+nb1-1];
        }
    }

  /*  sf_fileclose(vel);
    sf_fileclose(grad1);
    sf_fileclose(grad2);
  */
    for (iz=nz; iz < nbz; iz++) {
       for (ix=0; ix < nbx; ix++) {
        v[iz][ix]  = v[nz-1][ix];
        vx[iz][ix] = vx[nz-1][ix];
        vz[iz][ix] = vz[nz-1][ix];
       }
     }  


    sf_floatread(sig,nt,inp);		
    
        /* 2nd order FD*/
          a   = sf_floatalloc2(nbx,nbz);
          b10 = sf_floatalloc2(nbx,nbz);
          b20 = sf_floatalloc2(nbx,nbz);
          b01 = sf_floatalloc2(nbx,nbz);
          b02 = sf_floatalloc2(nbx,nbz);
           
    #ifdef _OPENMP
    #pragma omp parallel
    nth = omp_get_num_threads();
    sf_warning("using %d threads",nth);
    #pragma omp for private(iz,ix,w1,w2,h1,h2,a1,a2,a3,a4) 
    /*     shared(nbz,nbx,v,dt,dx,dz,vx,vz,a,b10,b20,b01,b02,cur,nxt) */
    #endif        
          for (iz=0; iz < nbz; iz++) {
          
              for (ix=0; ix < nbx; ix++) {
	          /* dimensionless velocity */
	           w1 = v[iz][ix] * dt/dx;
	           w2 = v[iz][ix] * dt/dz;
	          /* dimensionless gradient */
	           h1 = 0.5 * vx[iz][ix] * dt;
	           h2 = 0.5 * vz[iz][ix] * dt;

	      a1 = w1*w1 * (1.0 + h1*h1);
	      a2 = w2*w2 * (1.0 + h2*h2);
	      a3= h1*w1;
	      a4= h2*w2;
	
              a[iz][ix] = 2.0-2.0*a1-2.0*a2;
              b10[iz][ix] = a1-a3; 
              b20[iz][ix] = a1+a3; 
              b01[iz][ix] = a2-a4; 
              b02[iz][ix] = a2+a4; 

	      /* initial conditions */
	      cur[iz][ix] = 0.;
	      nxt[iz][ix] = 0.;
              }
           }
           free(v);
           free(vx);

          /* propagation in time */
          for (it=0; it < nt; it++) {

          #ifdef _OPENMP
          #pragma omp parallel for private(iz,ix) 
	      /* shared(nbz,nbx,old,cur,nxt) */
          #endif        
              for (iz=0; iz < nbz; iz++) { 
          
	         for (ix=0; ix < nbx; ix++) {
	              old[iz][ix] = cur[iz][ix];
	              cur[iz][ix] = nxt[iz][ix];
                     }   
                  }  
            
	  /* Stencil */
          nxt[0][0] = cur[0][0]*a[0][0] + cur[0][1]*b20[0][0] + cur[1][0]*b02[0][0]-old[0][0];
          nxt[0][nbx-1] = cur[0][nbx-1]*a[0][nbx-1] + cur[0][nbx-2]*b10[0][nbx-1] + cur[1][nbx-1]*b02[0][nbx-1]-old[0][nbx-1];
          nxt[nbz-1][0] = cur[nbz-1][0]*a[nbz-1][0] + cur[nbz-1][1]*b20[nbz-1][0] + cur[nbz-2][0]*b01[nbz-1][0]-old[nbz-1][0];
          nxt[nbz-1][nbx-1] = cur[nbz-1][nbx-1]*a[nbz-1][nbx-1] + cur[nbz-1][nbx-2]*b10[nbz-1][nbx-1] + cur[nbz-2][nbx-1]*b01[nbz-1][nbx-1]-old[nbz-1][nbx-1];
	  for (ix=1; ix < nbx-1; ix++) 
             { nxt[0][ix] = cur[0][ix]*a[0][ix] + cur[0][ix-1]*b10[0][ix] +
                            cur[0][ix+1]*b20[0][ix] + cur[1][ix]*b02[0][ix] - old[0][ix]; 
               nxt[nbz-1][ix] = cur[nbz-1][ix]*a[nbz-1][ix] + cur[nbz-1][ix-1]*b10[nbz-1][ix] +
                                cur[nbz-1][ix+1]*b20[nbz-1][ix] + cur[nbz-2][ix]*b01[nbz-1][ix] -old[nbz-1][ix];
              }
          nxt[0][sl+nb1] += sig[it]; /* Source Location */

          #ifdef _OPENMP
          #pragma omp parallel for private(iz,ix)\
                      shared(nbz,nbx,old,cur,nxt,a,b10,b20,b01,b02)
          #endif        
          for (iz=1; iz < nbz-1; iz++) { 
          nxt[iz][0] = 0.;
          nxt[iz][nbx-1] = 0.;
	  for (ix=1; ix < nbx-1; ix++) {
	      nxt[iz][ix] = cur[iz][ix]*a[iz][ix] + cur[iz][ix-1]*b10[iz][ix] + 
                            cur[iz][ix+1]*b20[iz][ix] + cur[iz-1][ix]*b01[iz][ix] + 
                            cur[iz+1][ix]*b02[iz][ix] - old[iz][ix];
              } 
	  }
	
          for (iz=0; iz < nb2; iz++) {
          for (ix=0; ix < nbx; ix++) {

               nxt[nz+iz][ix] *= exp(-lmd*iz*iz*lmd);
	       /*  nxt[nz+iz][ix] *= exp(-lmd*lmd*(iz+1)*(iz+1)); */
              }
          }
          #ifdef _OPENMP
          #pragma omp parallel for private(iz,ix)\
                    shared(nbz,nb1,nx,lmd,nxt)
          #endif        
          for (iz=0; iz < nbz; iz++) {
          for (ix=0; ix < nb1; ix++) {
              /* nxt[iz][nb1-1-ix] *= exp(-lmd*lmd*(nb1-ix-1)*(nb1-ix-1)); */
	      /* nxt[iz][nb1+nx+ix] *= exp(-lmd*lmd*(nb1-ix-1)*(nb1-ix-1)); */
              nxt[iz][nb1-1-ix] *= exp(-lmd*ix*ix*lmd);
              nxt[iz][nb1+nx+ix] *= exp(-lmd*ix*ix*lmd);
              }
          }
	  /* if (it < 500) {nxt[nb] = sig[it];} */
         
	 for (iz=0; iz < nz; iz++) {
         sf_floatwrite(nxt[iz]+nb1,nx,out);
         }
       }
        
    free(nxt);
    free(old);
    free(cur);
    free(sig);
    free(a);
    free(b10);
    free(b20);
    free(b01);
    free(b02);

    exit(0);
}
