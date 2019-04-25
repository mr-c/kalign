
#include "alignment_parameters.h"

int set_subm_gaps(struct parameters* param, struct aln_param* ap);

struct aln_param* init_ap(struct parameters* param,int numseq)
{
        struct aln_param* ap = NULL;
        int i,j;


        MMALLOC(ap, sizeof(struct aln_param));

        ap->tree = NULL;
        MMALLOC(ap->tree, sizeof(int) * (numseq*3+1));

        ap->subm = NULL;
        MMALLOC(ap->subm,sizeof (float*) * 32);
        for (i = 32;i--;){
                ap->subm[i] = NULL;
                MMALLOC(ap->subm[i],sizeof(float) * 32);
                for (j = 32;j--;){
                        ap->subm[i][j] = 0.0f;
                }
        }
        RUN(set_subm_gaps(param, ap));

        return ap;
ERROR:
        free_ap(ap);
        return NULL;
}

void free_ap(struct aln_param* ap)
{
        int i;
        if(ap){
                if(ap->subm){
                        for (i = 32;i--;){
                                MFREE(ap->subm[i]);
                        }
                        MFREE(ap->subm);
                }
                if(ap->tree){
                        MFREE(ap->tree);
                }
                MFREE(ap);
        }
}


int set_subm_gaps(struct parameters* param, struct aln_param* ap)
{
        int i,j;
        int m_pos = 0;
        short *matrix_pointer = 0;
        short blosum50mt[]={
                5,
                -2,  5,
                -1, -3, 13,
                -2,  5, -4,  8,
                -1,  1, -3,  2,  6,
                -3, -4, -2, -5, -3,  8,
                0, -1, -3, -1, -3, -4,  8,
                -2,  0, -3, -1,  0, -1, -2, 10,
                -1, -4, -2, -4, -4,  0, -4, -4,  5,
                -1,  0, -3, -1,  1, -4, -2,  0, -3,  6,
                -2, -4, -2, -4, -3,  1, -4, -3,  2, -3,  5,
                -1, -3, -2, -4, -2,  0, -3, -1,  2, -2,  3,  7,
                -1,  4, -2,  2,  0, -4,  0,  1, -3,  0, -4, -2,  7,
                -1, -2, -4, -1, -1, -4, -2, -2, -3, -1, -4, -3, -2, 10,
                -1,  0, -3,  0,  2, -4, -2,  1, -3,  2, -2,  0,  0, -1,  7,
                -2, -1, -4, -2,  0, -3, -3,  0, -4,  3, -3, -2, -1, -3,  1,  7,
                1,  0, -1,  0, -1, -3,  0, -1, -3,  0, -3, -2,  1, -1,  0, -1,  5,
                0,  0, -1, -1, -1, -2, -2, -2, -1, -1, -1, -1,  0, -1, -1, -1,  2,  5,
                0, -4, -1, -4, -3, -1, -4, -4,  4, -3,  1,  1, -3, -3, -3, -3, -2,  0,  5,
                -3, -5, -5, -5, -3,  1, -3, -3, -3, -3, -2, -1, -4, -4, -1, -3, -4, -3, -3, 15,
                -1, -1, -2, -1, -1, -2, -2, -1, -1, -1, -1, -1, -1, -2, -1, -1, -1,  0, -1, -3, -1,
                -2, -3, -3, -3, -2,  4, -3,  2, -1, -2, -1,  0, -2, -3, -1, -1, -2, -2, -1,  2, -1,  8,
                -1,  2, -3,  1,  5, -4, -2,  0, -3,  1, -3, -1,  0, -1,  4,  0,  0, -1, -3, -2, -1, -2,  5};

        short blosum62mt[]={
                40,
                -20,  40,
                0, -30,  90,
                -20,  40, -30,  60,
                -10,  10, -40,  20,  50,
                -20, -30, -20, -30, -30,  60,
                0, -10, -30, -10, -20, -30,  60,
                -20,  0, -30, -10,  0, -10, -20,  80,
                -10, -30, -10, -30, -30,  0, -40, -30,  40,
                -10,  0, -30, -10,  10, -30, -20, -10, -30,  50,
                -10, -40, -10, -40, -30,  0, -40, -30,  20, -20,  40,
                -10, -30, -10, -30, -20,  0, -30, -20,  10, -10,  20,  50,
                -20,  30, -30,  10,  0, -30,  0,  10, -30,  0, -30, -20,  60,
                -10, -20, -30, -10, -10, -40, -20, -20, -30, -10, -30, -20, -20,  70,
                -10,  0, -30,  0,  20, -30, -20,  0, -30,  10, -20,  0,  0, -10,  50,
                -10, -10, -30, -20,  0, -30, -20,  0, -30,  20, -20, -10,  0, -20,  10,  50,
                10,  0, -10,  0,  0, -20,  0, -10, -20,  0, -20, -10,  10, -10,  0, -10,  40,
                0, -10, -10, -10, -10, -20, -20, -20, -10, -10, -10, -10,  0, -10, -10, -10,  10,  50,
                0, -30, -10, -30, -20, -10, -30, -30,  30, -20,  10,  10, -30, -20, -20, -30, -20,  0,  40,
                -30, -40, -20, -40, -30,  10, -20, -20, -30, -30, -20, -10, -40, -40, -20, -30, -30, -20, -30, 110,
                0, -10, -20, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -20, -10, -10,  0,  0, -10, -20, -10,
                -20, -30, -20, -30, -20,  30, -30,  20, -10, -20, -10, -10, -20, -30, -10, -20, -20, -20, -10,  20, -10,  70,
                -10,  10, -30,  10,  40, -30, -20,  0, -30,  10, -30, -10,  0, -10,  30,  0,  0, -10, -20, -30, -10, -20,  40};

        short gon250mt[]={
                24,
                0,   0,
                5,   0, 115,
                -3,   0, -32,  47,
                0,   0, -30,  27,  36,
                -23,   0,  -8, -45, -39,  70,
                5,   0, -20,   1,  -8, -52,  66,
                -8,   0, -13,   4,   4,  -1, -14,  60,
                -8,   0, -11, -38, -27,  10, -45, -22,  40,
                -4,   0, -28,   5,  12, -33, -11,   6, -21,  32,
                -12,   0, -15, -40, -28,  20, -44, -19,  28, -21,  40,
                -7,   0,  -9, -30, -20,  16, -35, -13,  25, -14,  28,  43,
                -3,   0, -18,  22,   9, -31,   4,  12, -28,   8, -30, -22,  38,
                3,   0, -31,  -7,  -5, -38, -16, -11, -26,  -6, -23, -24,  -9,  76,
                -2,   0, -24,   9,  17, -26, -10,  12, -19,  15, -16, -10,   7,  -2,  27,
                -6,   0, -22,  -3,   4, -32, -10,   6, -24,  27, -22, -17,   3,  -9,  15,  47,
                11,   0,   1,   5,   2, -28,   4,  -2, -18,   1, -21, -14,   9,   4,   2,  -2,  22,
                6,   0,  -5,   0,  -1, -22, -11,  -3,  -6,   1, -13,  -6,   5,   1,   0,  -2,  15,  25,
                1,   0,   0, -29, -19,   1, -33, -20,  31, -17,  18,  16, -22, -18, -15, -20, -10,   0,  34,
                -36,   0, -10, -52, -43,  36, -40,  -8, -18, -35,  -7, -10, -36, -50, -27, -16, -33, -35, -26, 142,
                0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                -22,   0,  -5, -28, -27,  51, -40,  22,  -7, -21,   0,  -2, -14, -31, -17, -18, -19, -19, -11,  41,   0,  78,
                0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};


        if(param->sub_matrix){
                if(byg_start(param->sub_matrix,"blosum62BLOSUM62") != -1){
                        matrix_pointer = blosum62mt;
                        //m_pos = 0;
                        //for (i = 0;i < 23;i++){
                        //	for (j = 0;j <= i;j++){
                        //		matrix_pointer[m_pos] = matrix_pointer[m_pos] * 10;
                        //		m_pos++;
                        //	}
                        //}
                        ap->gpo = 55;
                        ap->gpe = 8;
                        ap->tgpe = 1;
                }
                if(byg_start(param->sub_matrix,"blosum50BLOSUM50") != -1){
                        matrix_pointer = blosum50mt;
                        m_pos = 0;
                        for (i = 0;i < 23;i++){
                                for (j = 0;j <= i;j++){
                                        matrix_pointer[m_pos] = matrix_pointer[m_pos] * 10;
                                        m_pos++;
                                }
                        }
                        ap->gpo = 55;
                        ap->gpe = 8;
                        ap->tgpe = 1;
                }
                //vogt....

        }else{
                //LOG_MSG("DNA>>>>> %d", param->dna);
                if(!param->dna){
                        // gpo:5.494941        gpe:0.852492        tgpe:0.442410       bonus: 3.408872     z-cutoff: 58.823309 -> 0.829257 accuracy on bb3
                        ap->gpo = 54.94941;
                        ap->gpe = 8.52492;
                        ap->tgpe = 4.42410;

                        //gpo = 54;
                        //gpe = 8;
                        //tgpe = 4;
                        //-gpo 10.9898        -gpe 0.852492      -tgpe  0.442410    -bonus    0.2   -zcutoff     58.823309
                        //	param->secret = 0.2;
                        matrix_pointer = gon250mt;
                }else{
                        //gpo = 400;
                        //	gpe =  30;
                        //tgpe = 30;

                        //param->gpo = 43.4;
                        //param->gpe = 3.94;
                        //param->tgpe = 29.26;

                        //gpo = 43.4 *5;
                        ap->gpo = 217;
                        ap->gpe = 39.4;
                        ap->tgpe =  292.6;
                        //param->secret = 28.3;
                        param->zlevel = 61.08;
                        param->internal_gap_weight = 49.14;

                }
        }
        if(param->gpo!= -1){
                //param->gpo *= 5;
                ap->gpo = param->gpo;
        }
        if(param->gpe != -1){
                //param->gpe *= 10;
                ap->gpe = param->gpe;
        }
        if(param->tgpe != -1){
                //param->tgpe *= 10;
                ap->tgpe = param->tgpe;
        }

//	if(param->secret != -1){
//		//param->secret *= 10;
//	}else{
        if(param->secret == -1){
                if(!param->dna){
                        param->secret = 0.2;
                }else{
                        param->secret = 283.0;
                }
        }


        //fprintf(stderr,"%d	%d	%d	%d\n",gpo,gpe,tgpe,	 (int)param->secret);

        for (i = 32;i--;){

                for (j = 32;j--;){
                        ap->subm[i][j] = param->secret;//0;//gpe << 1;//-5;// better on Balibase
                }
        }
        if(param->dna){
                /*subm[0][0] += 10;
                  subm[0][1] += 6;
                  subm[1][0] += 6;
                  subm[1][1] += 10;
                  subm[2][2] += 10;
                  subm[2][3] += 6;
                  subm[3][2] += 6;
                  subm[3][3] += 10;*/
//		     A    C    G    T    .    N
//	A   91 -114  -31 -123    0  -43
                ap->subm[0][0] += 91;
                ap->subm[0][1] += -114;
                ap->subm[0][2] += -31;
                ap->subm[0][3] += -123;

//	C -114  100 -125  -31    0  -43
                ap->subm[1][0] += -114;
                ap->subm[1][1] += 100;
                ap->subm[1][2] += -125;
                ap->subm[1][3] += -31;

//	G  -31 -125  100 -114    0  -43
                ap->subm[2][0] += -31;
                ap->subm[2][1] += -125;
                ap->subm[2][2] += 100;
                ap->subm[2][3] += -114;

//	T -123  -31 -114   91    0  -43
                ap->subm[3][0] += -123;
                ap->subm[3][1] += -31;
                ap->subm[3][2] += -114;
                ap->subm[3][3] += 91;

//	.    0    0    0    0    0    0
//	N  -43  -43  -43  -43    0  -43


                /*for (i = 0; i < 4;i++){
                  for (j = 0;j < 4;j++){
                  if(i == j){
                  subm[i][j] += 1;
                  }else{
                  subm[i][j] -= 3;
                  }
                  }
                  }*/

        }else{

                m_pos = 0;
                for (i = 0;i < 23;i++){
                        for (j = 0;j <= i;j++){
                                if (i == j){
                                        //	subm[i][j] += blosum62mt[m_pos]*10;
                                        ap->subm[i][j] += matrix_pointer[m_pos];
                                }else{
                                        //	subm[i][j] += blosum62mt[m_pos]*10;
                                        //	subm[j][i] += blosum62mt[m_pos]*10;
                                        ap->subm[i][j] += matrix_pointer[m_pos];
                                        ap->subm[j][i] += matrix_pointer[m_pos];
                                }
                                m_pos++;
                        }
                }
                /*for (i = 0; i < 23;i++){
                  for (j = 0; j < 23;j++){
                  fprintf(stderr,"%d ",subm[i][j]);
                  }
                  fprintf(stderr,"\n");
                  }
                  fprintf(stderr,"\n");*/
        }


float balimt[]={
 1.831635,
 0.379316, 8.837722,
 1.168681, -3.385540, 4.604774,
 0.160227, 3.005161, -0.577872, 2.524931,
 0.561321, 1.556723, -0.672084, 1.568317, 2.093489,
 -0.043455, -1.578632, 0.396961, -0.738931, -0.613133, 2.924703,
 0.625183, 0.070418, -0.226375, 0.219136, -0.089304, -0.741979, 2.636475,
 0.118435, -0.430333, -0.015706, 0.579807, 0.555900, 0.619931, -0.191916, 3.560293,
 0.281117, -1.656072, 0.553314, -1.002079, -0.602622, 0.990249, -1.150259, -0.355963, 2.198811,
 0.589559, 1.277425, -0.557440, 0.710139, 1.257084, -0.556258, 0.080996, 0.691413, -0.389008, 2.062445,
 0.239474, -2.063106, 0.467579, -0.781387, -0.420664, 1.422313, -0.984345, 0.053863, 1.755495, -0.138655, 1.934184,
 0.498295, -0.436613, 0.805282, -0.522882, -0.069955, 1.319529, -0.607032, 0.209326, 1.382541, 0.192607, 1.899450, 3.088760,
 0.308343, 2.671374, 0.115018, 1.453058, 0.753617, -0.293388, 0.700800, 1.198097, -0.635728, 0.867946, -0.280386, 0.027766, 2.583479,
 0.401810, -0.766759, -0.074592, 0.296412, 0.435010, -0.172066, -0.001469, 0.076393, -0.340843, 0.347929, -0.246365, -0.221469, 0.256587, 3.148435,
 0.577783, 0.796777, -0.171601, 0.862158, 1.475842, -0.147552, -0.041844, 1.091249, -0.220745, 1.429340, 0.176672, 0.657928, 0.910975, 0.291007, 2.443255,
 0.248191, -0.877328, -0.163101, 0.198318, 0.687208, -0.411668, -0.315114, 0.921150, -0.312021, 1.726771, -0.074459, 0.168556, 0.622920, 0.058703, 1.229882, 2.683812,
 1.220616, 1.479545, 0.723007, 0.749198, 0.640344, -0.196453, 0.607337, 0.445585, -0.372717, 0.625978, -0.288612, 0.189726, 1.078434, 0.592033, 0.711780, 0.410192, 2.018958,
 0.733006, 0.900417, 0.853584, 0.348481, 0.457251, 0.031104, -0.051004, 0.360130, 0.365788, 0.582263, 0.175219, 0.547117, 0.721819, 0.241165, 0.615165, 0.390010, 1.558641, 2.299288,
 0.777525, -2.015383, 1.128609, -0.812975, -0.368542, 0.706414, -0.952112, -0.136083, 2.199784, -0.118651, 1.263383, 1.036635, -0.407375, -0.056820, -0.026015, -0.192393, -0.038339, 0.861594, 2.030282,
 -0.212036, -1.512195, -0.121144, -0.473168, -0.444192, 1.854957, -0.670994, 0.900037, 0.170312, -0.400341, 0.497390, 0.718014, -0.190152, -0.464568, -0.002972, -0.091080, -0.054719, -0.109885, 0.055820, 4.896238,
 0.879746, 1.290848, 1.415761, 0.180268, 0.437218, 1.496035, -0.148757, -0.322519, 1.916862, 0.621864, 1.701643, 2.313120, 0.311163, -1.317975, 0.562863, 0.071397, 1.017557, 0.655439, 1.531601, -0.048335, 2.047755,
 0.011966, 0.140509, 0.150436, -0.380131, -0.328586, 2.345822, -0.639105, 1.435440, 0.264206, -0.122512, 0.642718, 0.699628, -0.031421, -0.312482, 0.207429, 0.198372, -0.007299, 0.041363, 0.224832, 2.082206, 0.432106, 3.253930,
 0.917017, 8.524478, -0.605286, 1.488840, 2.565214, -2.272047, -0.125547, -0.263343, -2.225519, 1.519710, -1.575455, -1.008318, 0.350579, -0.024788, 3.795967, -0.878750, 0.800069, 0.670941, -0.633799, -5.052065, 0.838441, -2.179794, 9.152690,
};
ap->gpo = 7.209365/2.0;
ap->gpe =  0.364304;
ap->tgpe =  0.0;

        m_pos = 0;
        for (i = 0;i < 23;i++){
                for (j = 0;j <= i;j++){
                        if (i == j){
                                //	subm[i][j] += blosum62mt[m_pos]*10;
                                ap->subm[i][j] = balimt[m_pos]*2.0;
                        }else{
                                //	subm[i][j] += blosum62mt[m_pos]*10;
                                //	subm[j][i] += blosum62mt[m_pos]*10;
                                ap->subm[i][j] = balimt[m_pos]*2.0;
                                ap->subm[j][i] = balimt[m_pos]*2.0;
                        }
                        m_pos++;
                }
        }


        return OK;
}
