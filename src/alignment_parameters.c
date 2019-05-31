
#include "alignment_parameters.h"

#include "alphabet.h"
int set_subm_gaps(struct aln_param* ap);
int set_subm_gaps_DNA(struct aln_param* ap);
int set_param_number(struct aln_param* ap,int L, int sel);
int read_aln_param_from_file(struct aln_param* ap, char* infile, int L);

struct aln_param* init_ap(int numseq,int L)
{
        struct aln_param* ap = NULL;
        int i,j;

        MMALLOC(ap, sizeof(struct aln_param));

        ap->tree = NULL;
        MMALLOC(ap->tree, sizeof(int) * (numseq*3+1));

        for(i = 0;i < (numseq*3+1);i++){
                ap->tree[i] = 0;
        }
        ap->subm = NULL;

        RUNP(ap->rng = init_rng(0));
        //srand48_r(time(NULL), &ap->randBuffer);
        MMALLOC(ap->subm,sizeof (float*) * 32);
        for (i = 32;i--;){
                ap->subm[i] = NULL;
                MMALLOC(ap->subm[i],sizeof(float) * 32);
                for (j = 32;j--;){
                        ap->subm[i][j] = 0.0f;
                }
        }
        //if(param->param_set == -1){

        if(L == defDNA){
                LOG_MSG("DNADNADNA");
                //exit(0);
                RUN(set_subm_gaps_DNA(ap));
        }else if(L == defPROTEIN){

                RUN(set_subm_gaps(ap));
        }else if(L == redPROTEIN){
                RUN(set_subm_gaps(ap));
        }
                //}else{
                //set_param_number(ap, L, param->param_set);
                //}
        /* read parameters from file */
        //if(param->aln_param_file){
                //LOG_MSG("Reading parameters from file: %s", param->aln_param_file);
                //RUN(read_aln_param_from_file(ap, param->aln_param_file, L));
                //}


        return ap;
ERROR:
        free_ap(ap);
        return NULL;
}

int read_aln_param_from_file(struct aln_param* ap, char* infile, int L)
{
        char line_buffer[BUFFER_LEN];
        FILE* f_ptr = NULL;
        double* values = NULL;
        int i,j;
        int m_pos;
        double val = 0;

        int expected;


        expected = (L * (L-1)) / 2 + L + 3;

        MMALLOC(values, sizeof(double) * expected); /* NEED to remove constant!!!  */

        if(!my_file_exists(infile)){
                ERROR_MSG("File: %s does not exist", infile);
        }
        RUNP( f_ptr = fopen(infile, "r"));

        m_pos =0;
        while (fgets(line_buffer , BUFFER_LEN, f_ptr)){
                sscanf(line_buffer, "%lf", &val);
                values[m_pos] = val;
                m_pos++;
                //fprintf(stdout,"%s", ret);
        }

        //fprintf(stdout,"-%s-", ret);


        if(expected != m_pos){
                ERROR_MSG("Expected number of parameters (%d) do not match number of parameters in file %s (%d)", expected,infile,m_pos);
        }

        m_pos = 0;
        for (i = 0;i < L;i++){
                for (j = 0;j <= i;j++){

                        ap->subm[i][j] = values[m_pos];
                        ap->subm[j][i] = ap->subm[i][j];
                        m_pos++;
                }
        }

        ap->gpo = values[m_pos];
        m_pos++;
        ap->gpe = values[m_pos];
        m_pos++;

        ap->tgpe = values[m_pos];

        MFREE(values);
        fclose(f_ptr);

        return OK;
ERROR:
        return FAIL;
}

int set_param_number(struct aln_param* ap,int L, int sel)
{
        int i,j,m_pos;
        double* ptr = NULL;

        double defprot_set10[4][234] = {{ 2.441135 ,0.804586,5.229447,-0.275312,-0.932531,3.078253,0.117964,-1.112335,1.203915,2.665036,-0.478196,0.062497,-1.199822,-1.106372,3.544699,0.243667,-0.629188,-0.166715,-0.522073,-1.419407,3.295848,-0.252812,-0.304885,0.225093,0.189015,0.248641,-0.700361,4.058245,-0.116301,0.221291,-1.419282,-1.003624,0.588717,-1.604453,-0.759925,2.735987,0.182665,-0.915737,0.321957,0.857994,-1.022471,-0.334935,0.297575,-0.784649,2.621895,-0.172126,0.172567,-1.153285,-0.827709,1.022751,-1.503650,-0.309763,1.354375,-0.528795,2.504544,0.090768,0.449192,-0.994454,-0.448665,0.881127,-1.049855,-0.151963,0.958608,-0.219735,1.535655,3.595027,-0.078957,-0.296744,1.113714,0.378894,-0.733749,0.377618,0.809122,-1.043225,0.560572,-0.659316,-0.266522,3.114087,0.077683,-0.717091,-0.075007,-0.003924,-0.752411,-0.544078,-0.270432,-0.714819,-0.032959,-0.715190,-0.657222,-0.232401,3.808283,0.209099,-0.576439,0.480587,1.082240,-0.622755,-0.442365,0.731248,-0.597657,1.065589,-0.192281,0.282779,0.572326,-0.163616,2.996754,-0.216587,-0.556078,-0.276231,0.165220,-0.918021,-0.771466,0.554353,-0.743857,1.327472,-0.543696,-0.286912,0.152924,-0.544813,0.760249,3.307940,0.862154,0.406596,0.359384,0.221433,-0.719529,0.199665,0.082347,-0.820916,0.216351,-0.743032,-0.184038,0.754032,0.186512,0.303803,-0.080536,2.664838,0.340064,0.423405,-0.046178,0.011205,-0.507827,-0.516927,-0.055173,-0.010057,0.160184,-0.246784,0.196552,0.363234,-0.210271,0.156528,-0.168975,1.174802,2.941623,0.405328,0.778105,-1.278921,-0.843631,0.290444,-1.474030,-0.519590,1.808021,-0.548076,0.844055,0.587147,-0.776456,-0.532748,-0.462248,-0.630362,-0.455832,0.518245,2.608431,-0.514309,-0.347037,-1.197980,-0.918742,1.470235,-1.258146,0.590844,-0.170494,-0.813930,0.109186,0.385784,-0.639783,-0.857979,-0.484561,-0.631388,-0.601573,-0.599450,-0.220994,5.505481,-0.357527,-0.191207,-0.925302,-0.866943,1.973688,-1.203284,1.101391,-0.080217,-0.573368,0.265681,0.335302,-0.388156,-0.855382,-0.290231,-0.315724,-0.517638,-0.432202,-0.138017,1.636025,3.872373,1.681627,0.937222,2.171531,-0.001955,-0.764385,-0.115077,-0.025456,0.620326,2.017909,0.064413,0.122257,0.283459,2.263544,0.488142,0.407935,0.793370,0.866565,0.434452,0.959704,-0.491955,15.191782,3.610458,0.378310,0.006965},
{ 2.375830 ,0.643607,5.204934,-0.249461,-1.173050,3.172340,0.192837,-1.161841,1.093372,2.710988,-0.401979,-0.156276,-1.082798,-0.903211,3.434700,0.146163,-0.753883,-0.281686,-0.506025,-0.867086,3.140448,-0.341275,-0.662201,0.089096,0.085106,0.143585,-0.504392,4.277457,-0.160461,-0.027477,-1.482790,-1.091093,0.585205,-1.579484,-0.771554,2.841786,0.153642,-1.213189,0.278857,0.860792,-0.897669,-0.341059,0.280853,-0.865861,2.698454,-0.171662,-0.185907,-1.303096,-0.882391,1.030682,-1.331878,-0.406372,1.341062,-0.610478,2.549526,0.074865,0.259681,-0.892696,-0.553258,1.001488,-1.028818,-0.273395,0.982579,-0.227282,1.441501,3.773014,-0.155071,-0.360285,0.950083,0.303635,-0.666252,0.153988,0.769154,-1.107452,0.331698,-0.746403,-0.521854,3.255696,-0.165589,-0.340985,-0.185337,0.059935,-0.340691,-0.326977,-0.467090,-0.829518,-0.133347,-0.614695,-0.668856,-0.099074,3.706510,0.103590,-0.720914,0.413633,1.047622,-0.446286,-0.538221,0.626237,-0.709933,0.965443,-0.296786,0.237723,0.417615,-0.120407,3.110847,-0.088261,-0.704532,-0.124559,0.455879,-0.703728,-0.701110,0.466465,-0.718099,1.318186,-0.419700,-0.184599,0.296735,-0.130674,0.927516,3.208160,0.758824,0.095418,0.309572,0.255178,-0.477662,0.166401,-0.056159,-0.792113,0.209069,-0.715987,-0.306894,0.537011,0.108416,0.257043,0.085939,2.590287,0.298809,0.438208,-0.109154,0.072396,-0.189854,-0.434899,-0.020938,-0.117846,0.157773,-0.237370,0.033414,0.220778,-0.150298,0.249437,0.158230,1.121004,2.827351,0.304373,0.589297,-1.217991,-0.745985,0.318079,-1.316178,-0.573843,1.760337,-0.547804,0.877044,0.668480,-0.902387,-0.401172,-0.437892,-0.580874,-0.477051,0.369209,2.616250,-0.735670,-0.888142,-0.490997,-0.757002,1.491052,-0.854143,0.408417,-0.289839,-0.822824,0.084759,0.237157,-0.467658,-0.908696,-0.278850,-0.281685,-0.225505,-0.363700,-0.517814,5.404440,-0.445265,-0.443508,-0.624691,-0.558171,1.873487,-0.883587,0.883846,-0.224340,-0.458380,0.207657,0.276651,-0.487803,-0.547611,-0.040354,-0.023209,-0.286858,-0.255100,-0.244120,1.783873,3.796554,0.284200,0.077413,1.186291,1.219788,0.347297,-0.586510,-0.620932,0.601297,0.662807,0.455680,1.132707,0.727649,-1.095943,1.930136,-0.521231,0.693074,0.349914,0.299677,-1.077936,-0.408848,5.696912,3.555584,0.356512,0.005053},
{ 2.988765 ,0.330658,6.048533,-0.372999,-1.742733,3.355200,-0.111936,-1.441599,1.001284,3.255966,-1.296498,-0.009322,-2.206448,-2.124382,3.641940,0.220432,-1.208191,-0.136338,-0.700479,-1.834093,3.354263,-0.806861,-0.518831,-0.391664,-0.448703,-0.138816,-1.000365,4.531540,-0.458200,-0.773621,-1.836550,-1.328077,0.249780,-1.869329,-1.983229,3.246271,0.098474,-0.668583,-0.220930,0.268151,-2.047742,-0.625460,-0.453621,-1.241727,3.605983,-0.828022,-0.213279,-2.056982,-1.590836,0.527728,-2.047133,-1.877396,1.103220,-1.258418,2.986776,-0.723798,-0.379368,-1.803284,-1.323239,0.214706,-1.797275,-1.082570,0.781156,-0.807643,1.257172,4.823113,-0.306136,-0.774508,0.994684,-0.109040,-1.280948,0.251180,0.419551,-0.988364,0.090954,-1.389571,-1.361113,2.994998,0.142169,-0.165729,-0.637585,-0.797158,-1.731014,-0.608888,-1.184747,-1.369715,-0.407386,-1.399763,-1.536961,-0.516908,4.004747,-0.114701,-0.494646,0.018146,0.971865,-1.456246,-0.569627,0.213520,-1.029848,1.136034,-0.937544,-0.255363,0.265655,-0.326582,3.448932,-0.251985,-0.585584,-0.438468,-0.072998,-1.351157,-0.826051,-0.161218,-1.252762,1.447143,-0.941115,-1.055739,0.048983,-0.655716,0.888946,3.372770,0.693722,0.213017,0.124399,-0.253277,-1.099570,0.154263,-0.763192,-0.919637,-0.284745,-1.078317,-0.839221,0.758804,0.096349,0.097942,-0.140681,2.760864,0.074674,-0.733432,-0.390260,-0.161833,-1.140534,-0.969146,-1.347766,-0.015379,0.093481,-0.775850,-0.118827,0.250024,-0.459992,0.046860,-0.033931,1.018917,3.083992,0.335871,0.106340,-1.755562,-1.113193,-0.327028,-1.692354,-1.970581,1.840384,-0.995567,0.403277,0.473676,-1.380651,-0.847685,-0.969741,-1.183394,-0.948561,0.127447,3.248558,-1.677426,-1.771605,-2.385338,-1.744890,0.198857,-2.256367,-0.465483,-1.811524,-2.492853,-1.188690,-0.640855,-2.791854,-1.573229,-1.866240,-1.559244,-1.814671,-1.882153,-1.824870,5.387924,-1.161668,-0.657621,-1.193655,-1.380692,1.584365,-1.809631,0.788082,-1.233994,-1.582458,-0.374156,-0.552714,-1.283069,-1.770388,-1.336656,-0.976387,-1.070930,-1.387361,-1.263470,-0.092070,3.852683,4.438058,7.271657,4.565167,4.479790,4.709276,4.265295,5.534752,4.712102,5.107466,3.930502,6.200284,4.618413,4.974178,5.182669,4.633832,4.421237,4.559339,4.531892,5.911335,4.786914,21.954597,3.794700,0.421863,0.027185},
{ 2.232931 ,1.057775,3.872678,0.169553,-0.103676,2.758993,0.369081,-0.135230,1.157821,2.383283,-0.102529,0.258417,-0.662975,-0.612489,3.413148,0.481787,0.212790,0.371571,-0.012080,-0.633158,3.039450,0.034057,0.225904,0.338794,0.225409,0.327077,-0.080396,3.825504,0.207216,0.644692,-0.632450,-0.372329,0.666708,-0.789520,-0.390353,2.659703,0.465312,0.091239,0.445652,0.797031,-0.503043,-0.014689,0.370923,-0.278195,2.259899,0.170175,0.572394,-0.537310,-0.297406,0.962193,-0.658364,-0.099282,1.418371,-0.142005,2.326511,0.519208,0.911975,-0.348098,-0.080380,0.760070,-0.352575,0.063337,1.177040,0.011250,1.395354,3.286165,0.249498,0.165238,1.087545,0.549153,-0.283970,0.578362,0.925880,-0.436958,0.606512,-0.314716,-0.114963,2.672350,0.409597,-0.035851,0.244759,0.313980,-0.474016,0.216863,0.023327,-0.386687,0.289280,-0.260558,-0.101728,0.311883,3.074851,0.426409,0.289590,0.647739,1.076847,-0.346301,0.161717,0.712282,-0.186318,0.991033,0.004614,0.271507,0.602110,0.276272,2.471184,0.182095,0.250206,0.127889,0.447480,-0.349322,-0.161525,0.481948,-0.249548,1.252403,-0.111172,0.079122,0.464390,0.106980,0.720093,2.869985,0.791158,0.874919,0.556542,0.393894,-0.239243,0.466838,0.444613,-0.230905,0.448122,-0.110508,0.187499,0.772252,0.578583,0.637750,0.358795,1.963403,0.524514,0.661450,0.403349,0.426756,-0.144736,0.083357,0.284615,0.237865,0.572066,0.139552,0.428595,0.585936,0.263856,0.524917,0.371733,1.144194,2.356782,0.596024,0.988593,-0.478686,-0.156852,0.535219,-0.513215,-0.227649,1.806518,-0.049600,0.986069,0.830472,-0.269083,-0.181403,0.058759,-0.143302,0.026346,0.457791,2.430302,-0.376166,0.414717,-0.715912,-0.599316,1.426047,-0.699126,0.215677,-0.055418,-0.275381,0.298257,0.272396,-0.601420,-0.669864,-0.210639,-0.313587,-0.512138,-0.328567,-0.068667,5.370957,-0.029636,0.429613,-0.408193,-0.443011,2.040433,-0.524910,1.127206,0.219616,-0.277388,0.324859,0.266845,-0.083663,-0.397247,-0.171474,-0.114692,-0.151616,-0.074518,0.231096,1.860899,3.592874,0.442121,1.658955,0.638475,0.303002,1.696762,-0.132350,0.415602,1.117820,0.167463,0.921516,2.669780,0.844726,0.133353,-0.747866,-0.440784,0.428838,-0.201140,0.162823,1.468113,1.364092,6.001722,3.747994,0.313172,-0.001827}
};
        ptr = defprot_set10[sel];
        m_pos = 0;
        for (i = 0;i < L;i++){
                for (j = 0;j <= i;j++){

                        ap->subm[i][j] = ptr[m_pos];
                        ap->subm[j][i] = ap->subm[i][j];
                        m_pos++;
                }
        }

        ap->gpo = ptr[m_pos];
        m_pos++;
        ap->gpe = ptr[m_pos];
        m_pos++;

        ap->tgpe = ptr[m_pos];

        return OK;
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
                if(ap->rng){
                        free_rng(ap->rng);
                }
                if(ap->tree){
                        MFREE(ap->tree);
                }
                MFREE(ap);
        }
}


int set_subm_gaps_DNA(struct aln_param* ap)
{
        int i,j;


        for(i = 0; i < 5; i++){

                for(j =0; j < 5;j++){
                        ap->subm[i][j] = 283;
                }
        }
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

        ap->gpo = 217;
        ap->gpe = 39.4;
        ap->tgpe =  292.6;
        //param->secret = 28.3;
 			  //		     A    C    G    T    .    N

        return OK;
}
int set_subm_gaps(struct aln_param* ap)
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

        /*-      float balimt[]={
                3.920888,
                1.604000, 7.678390,
                0.382178, -1.189945, 5.110847,
                1.308758, -1.166734, 2.426962, 5.181517,
                0.107950, 0.924254, -1.659644, -1.033091, 5.887682,
                0.921866, -1.281853, 0.517505, -0.172994, -1.825433, 5.662972,
                0.420678, 0.147276, 0.842685, 0.294010, 0.988670, -0.692163, 6.409919,
                0.283391, 1.010031, -2.810585, -1.510052, 2.156337, -3.083201, -0.557852, 3.780869,
                1.363513, -2.938934, 0.702696, 2.701106, -0.941031, 0.349078, 1.568294, -0.589172, 5.142984,
                0.670243, 0.174749, -1.374346, -1.589740, 2.792841, -2.033580, -0.177918, 3.200343, -0.090338, 4.882996,
                0.706894, 0.815683, -0.975458, -0.948137, 2.805333, -1.033110, 0.108622, 2.482203, -0.414479, 3.977264, 6.176908,
                0.784338, 0.380946, 1.344116, 0.693213, -0.640990, 1.571191, 1.713356, -1.577401, 1.805460, -0.614647, -0.191263, 5.635500,
                0.983142, -0.998642, 0.661274, 0.934279, -1.784953, 0.029836, 0.083552, -1.502876, 0.862314, -1.309423, -1.273661, -0.293410, 6.225654,
                1.149429, -0.276232, 1.457434, 2.616933, -0.106710, -0.383396, 1.395446, -0.285260, 2.045882, -0.222200, 1.512061, 2.012131, 0.517603, 4.654614,
                0.685424, -0.685493, 0.034906, 1.541380, -0.638209, -0.544553, 2.011307, -0.684741, 3.547955, 0.041426, 0.516878, 0.445455, -0.178038, 2.535896, 5.763936,
                1.619906, 1.575984, 1.689382, 1.470300, -0.202978, 0.925715, 1.082738, -0.559612, 0.441048, -0.888255, 0.081922, 1.342780, 1.371791, 1.117049, 0.392311, 5.095183,
                1.650149, 1.661059, 0.799802, -0.768239, 0.197150, 0.064081, 0.413027, 0.904800, 1.298174, -0.459154, 0.291705, 1.495749, -0.199729, 1.174144, 0.727001, 3.261519, 5.132018,
                1.245536, 2.403317, -1.437802, -0.670541, 1.586491, -2.700304, -0.902244, 3.644172, -0.293981, 2.601818, 1.726683, -1.125613, -0.049767, -0.332525, -0.219708, 0.108320, 1.903368, 4.367546,
                -0.262759, -0.194028, -1.236521, -1.699648, 3.504898, -1.148102, 1.205952, -0.481433, -0.620752, 0.915806, 1.080921, -0.307369, -1.196403, -0.106593, -0.242729, 0.012081, -0.125929, 0.266423, 7.527638,
                0.210322, 0.446633, -0.686575, -0.896346, 4.430549, -1.111351, 2.049135, -0.253197, -0.085094, 1.248847, 0.586817, -0.381326, -0.431627, 0.595602, 0.524817, 0.163105, 0.030212, 0.251299, 4.313012, 5.837501,
                1.319698, 1.459442, 2.336812, 3.278178, 1.513078, -0.151524, -1.199513, 1.741649, 2.183355, 1.049455, 3.233596, 1.690985, -1.284263, 4.359771, -1.115270, 2.350918, 1.658454, 1.654000, -1.339485, -0.314211, 8.108932,
        };
        ap->gpo = 4.596402;
        ap->gpe =  0.037633;
        ap->tgpe =  0.004585;
*/
float balimt[]={
 4.920888,
 2.604000, 8.678390,
 1.161502, -0.189945, 6.080085,
 1.808781, -0.229204, 3.426962, 6.119047,
 1.107950, 1.924254, -1.159621, -0.033091, 6.764633,
 1.921866, -0.282280, 1.079998, 0.827006, -0.833215, 6.662880,
 1.170705, 1.147276, 1.842685, 0.294025, 1.519913, 0.307837, 7.409705,
 1.283391, 2.010031, -1.810585, -0.510052, 3.156337, -2.083201, 0.442148, 4.780869,
 2.363513, -1.938934, 1.702696, 3.701106, 0.031625, 1.349078, 2.552699, 0.410828, 6.142038,
 1.545272, 1.174749, -0.624319, -0.589740, 3.789026, -1.158551, 0.806487, 4.200343, 0.909662, 5.879120,
 1.706894, 1.814737, 0.024542, 0.051863, 3.801457, -0.033110, 1.081278, 3.357232, 0.460550, 4.852293, 7.176908,
 1.768835, 1.380946, 2.344085, 1.443240, 0.357087, 1.571298, 2.650886, -0.577401, 2.805460, 0.322883, 0.746267, 6.635469,
 1.983112, 0.001358, 1.657490, 1.692088, -0.785167, 1.029836, 1.083552, -0.502876, 1.862314, -0.313299, -0.336131, 0.706590, 7.225196,
 2.148483, 0.238317, 2.457343, 3.616475, 0.830820, 0.120289, 2.391662, 0.214763, 2.795909, 0.777769, 2.508185, 3.012100, 1.517603, 5.654614,
 1.576078, 0.314507, 1.034906, 2.291407, -1.638209, 0.455447, 2.884382, 0.315259, 4.532360, 0.510198, 1.516878, 0.945936, 0.821962, 3.039795, 6.763936,
 2.619906, 2.575984, 2.687459, 2.220327, 0.548971, 1.425952, 1.582761, 0.440388, 1.441048, 0.109822, 0.832162, 2.342780, 2.371791, 2.114028, 1.390877, 6.095183,
 2.150172, 2.660601, 1.795896, -0.018212, 1.181555, 0.064829, 1.350557, 1.842330, 2.235704, 0.540846, 1.260485, 2.491873, 0.800271, 2.174144, 1.721142, 4.261519, 6.130065,
 2.244834, 3.403317, -0.437802, 0.329459, 2.586491, -1.700304, 0.089974, 4.643226, 0.706019, 3.601818, 2.726683, -0.246709, 0.934638, 0.667444, 0.780292, 1.108320, 2.887773, 5.367546,
 0.252859, 0.805972, -0.236521, -0.699648, 4.504898, -0.148102, 2.205952, 0.471234, 0.379248, 1.915806, 2.078998, 0.692631, -0.196403, 0.643434, 0.753395, 1.012081, 0.374094, 1.239079, 8.527638,
 1.210322, 1.446633, 0.250924, 0.103654, 5.313360, -0.111351, 3.049135, 0.746803, 0.914906, 2.247382, 1.586787, 0.368701, 0.568373, 1.595602, 1.524817, 1.163105, 1.030212, 1.249346, 5.188041, 6.837501,
 2.288478, 2.459442, 3.336812, 3.778201, 2.507219, 0.848476, -0.199513, 2.741649, 3.181432, 2.049455, 4.233230, 2.566014, -0.284263, 5.328551, -0.240241, 3.304073, 2.656531, 2.653054, -0.339485, 0.685789, 9.108932,
};
ap->gpo = 5.112020;
ap->gpe =  0.058520;
ap->tgpe =  0.007067;

        m_pos = 0;
        for (i = 0;i < 21;i++){
                for (j = 0;j <= i;j++){
                        if (i == j){
                                //	subm[i][j] += blosum62mt[m_pos]*10;
                                ap->subm[i][j] = balimt[m_pos];
                        }else{
                                //	subm[i][j] += blosum62mt[m_pos]*10;
                                //	subm[j][i] += blosum62mt[m_pos]*10;
                                ap->subm[i][j] = balimt[m_pos];
                                ap->subm[j][i] = balimt[m_pos];
                        }
                        m_pos++;
                }
        }

        return OK;
}
