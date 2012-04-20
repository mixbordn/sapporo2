#include "sapporohostclass.h"

struct real3 {
  double x, y, z;
};

int main(int argc, char *argv[]) {
  int n = 1024;
  if (argc > 1) n = atoi(argv[1]);
  cerr << " n = " << n << endl;

  int nx = 1;
  int n1 = 0;
  int n2 = n;

  double (*pos)[3] = new double[n][3];
  double (*vel)[3] = new double[n][3];
  double (*acc)[3] = new double[n][3];
  double (*jrk)[3] = new double[n][3];
  double *pot  = new double[n];
  double *mass = new double[n];
  int    *nnb  = new int[n];
  double *h2  = new double[n];
  int    *nngb  = new int[n];
  int    *ngb_list = new int[n];
  int    *id   = new int[n];

  double tm = 0;
  for (int i = 0; i < n; i++) {
    pos[i][0] = drand48();
    pos[i][1] = drand48();
    pos[i][2] = drand48();

    vel[i][0] = drand48();
    vel[i][1] = drand48();
    vel[i][2] = drand48();
 
    h2[i] = 1.0 * pow(12.0/n, 1.0/3);
    h2[i] = h2[i]*h2[i];

    mass[i] = 1.0/n * drand48();
    tm += mass[i];
    id[i] = i + 1;
  }

  for (int i = 0; i < n; i++) {
    mass[i] *= 1.0/tm;
  }
  
  sapporo grav;
  
  int cluster_id;
//   int sapporo::open(std::string kernelFile, int *devices, int nprocs = 1, int order = FOURTH)
//   grav.open("CUDA/kernels4th.ptx", cluster_id, 1, 1);
  int devices[] = {0,1,2,3};
//  grav.open("CUDA/kernels4thDP.ptx",devices , 1, 1);
  grav.open("CUDA/kernels4th.ptx",devices , 1, 1);
  
  int ipmax = grav.get_n_pipes();
  double *i_nene = new double[ipmax];


  double null3[3] = {0,0,0};
  for (int i = 0; i < n; i++) {
    grav.set_j_particle(i, id[i],
			0, 0,
			mass[i], null3, null3, null3,
			vel[i], pos[i], null3, null3, 0);
  }
  
  grav.set_time(0);
  
  double eps2 = 0;
  n1 = 0;
  n2 = 2*ipmax;
  for (int i = n1; i < n2; i += ipmax) {
    int npart = min(n2 - i, ipmax);
    
    grav.startGravCalc(n, npart,
			id + i,
			pos+i, vel+i,
			acc+i, jrk+i, pot+i, eps2, h2, NULL);
                        
    grav.getGravResults(n, npart,
			id+i, pos+i, vel+i,
			eps2, h2,
			acc+i, jrk+i,NULL, NULL, pot+i, nnb+i, NULL, true);                        
  }

  for (int kk = 0; kk < 1; kk++) {
    fprintf(stderr, "Computing forces on GPU\n");
    double t1 = get_time();
    int n_evaluate = 0;

    for (int cycle = 0; cycle < 100; cycle++)
    {
      for(int nipart = 1; nipart <= ipmax; nipart++)
      {
        int an = 1024;
        int n1 = int(an*drand48());
        int n2 = int(an*drand48());
        if (n2 < n1) {
          int tmp = n2;
          n2 = n1;
          n1 = tmp;
        }
        
        n1 = 0; 
        n2 = nipart;

        n_evaluate += n2 - n1;
        fprintf(stderr, "cycle= %d dn= %d .... ", cycle, n2-n1);
        for (int i = n1; i < n2; i++) {
          grav.set_j_particle(i, id[i],
                              0, 0,
                              mass[i], null3, null3, null3,
                              vel[i], pos[i], null3, null3, 0);
        }
    //     fprintf(stderr, "n1= %d n2= %d \n", n1, n2);
        grav.set_time(0);
        double tx = get_time();
        for (int i = n1; i < n2; i += ipmax) {
          int npart = min(n2 - i, ipmax);
    
        int ntest = n;   
          grav.startGravCalc(ntest, npart,
                              id + i,
                              pos+i, vel+i,
                              acc+i, jrk+i, pot+i, eps2, h2, NULL);

          grav.getGravResults(ntest, npart,
                            id+i, pos+i, vel+i,
                            eps2, h2,
                            acc+i, jrk+i, NULL, NULL, pot+i, NULL, NULL, false);
                            
        
    
          }
        

        double dt = get_time() - tx;
        double dt1 = get_time() - t1;
        double gflop1 = 60.0e-9*(n2 - n1)*n/dt;
        double gflop2 = 60.0e-9*n_evaluate*n/dt1;
        fprintf(stderr,"  GFLOP/s: %g %g \n", gflop1, gflop2);
      }
    }
  }
  grav.close();
 

 
  cerr << "done!\n";
}
