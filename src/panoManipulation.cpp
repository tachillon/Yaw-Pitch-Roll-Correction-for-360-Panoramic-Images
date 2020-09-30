#include "panoManipulation.h"

#define DEG_TO_RAD( x ) ( (x) * 2.0 * PI / 360.0)
#define PI_FLOAT    3.14159265f
#define PIBY2_FLOAT 1.5707963f
#define SQRT2       1.4142135623730951
#define PI          3.14159265358979323846264338327950288

// function convert second into day  
// hours, minutes and seconds 
string ConvertSectoDay(int n) 
{ 
    int day = n / (24 * 3600); 
  
    n = n % (24 * 3600); 
    int hour = n / 3600; 
  
    n %= 3600; 
    int minutes = n / 60 ; 
  
    n %= 60; 
    int seconds = n; 
      
    stringstream ss;
    ss << day << " " << "days " << hour  
       << " " << "hours " << minutes << " " 
       << "minutes " << seconds << " " 
       << "seconds. "  << endl; 

    return ss.str();
} 

float fast_sqrt(float x)
{
	return sqrt(x);
}

void matrix_matrix_mult( float m1[3][3],float m2[3][3],float result[3][3])
{
	result[0][0] =  m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0]; //i = 0 && k = 0
	result[0][1] =  m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1]; //i = 0 && k = 1
	result[0][2] =  m1[0][0] * m2[0][2] + m1[0][1] * m2[1][2] + m1[0][2] * m2[2][2]; //i = 0 && k = 2

	result[1][0] =  m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0]; //i = 1 && k = 0
	result[1][1] =  m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1]; //i = 1 && k = 1
	result[1][2] =  m1[1][0] * m2[0][2] + m1[1][1] * m2[1][2] + m1[1][2] * m2[2][2]; //i = 1 && k = 2

	result[2][0] =  m1[2][0] * m2[0][0] + m1[2][1] * m2[1][0] + m1[2][2] * m2[2][0]; //i = 2 && k = 0
	result[2][1] =  m1[2][0] * m2[0][1] + m1[2][1] * m2[1][1] + m1[2][2] * m2[2][1]; //i = 2 && k = 1
	result[2][2] =  m1[2][0] * m2[0][2] + m1[2][1] * m2[1][2] + m1[2][2] * m2[2][2]; //i = 2 && k = 2
}

void matrix_inv_mult( float m[3][3], float vector[3] )
{
	register float v0 = vector[0];
	register float v1 = vector[1];
	register float v2 = vector[2];
	//

	vector[0] =  m[0][0] * v0 + m[1][0] * v1 + m[2][0] * v2;
	vector[1] =  m[0][1] * v0 + m[1][1] * v1 + m[2][1] * v2;
	vector[2] =  m[0][2] * v0 + m[1][2] * v1 + m[2][2] * v2;
}

void SetMatrix( float a, float b, float c , float m[3][3], int cl )
{
        float mx[3][3], my[3][3], mz[3][3], dummy[3][3];

        // Calculate Matrices;

        mx[0][0] = 1.0f;                        mx[0][1] = 0.0f;                        mx[0][2] = 0.0f;
        mx[1][0] = 0.0f;                        mx[1][1] = (float)cos(a) ;              mx[1][2] = (float)sin(a);
        mx[2][0] = 0.0f;                        mx[2][1] =-mx[1][2] ;                   mx[2][2] = mx[1][1];
        
        my[0][0] = (float)cos(b);               my[0][1] = 0.0f;                        my[0][2] = (float)-sin(b);
        my[1][0] = 0.0f;                        my[1][1] = 1.0f;                        my[1][2] = 0.0f;
        my[2][0] = -my[0][2];                   my[2][1] = 0.0f;                        my[2][2] = my[0][0];
        
        mz[0][0] = (float)cos(c) ;              mz[0][1] = (float)sin(c);               mz[0][2] = 0.0f;
        mz[1][0] =-mz[0][1] ;                   mz[1][1] = mz[0][0] ;                   mz[1][2] = 0.0f;
        mz[2][0] = 0.0f;                        mz[2][1] = 0.0f;                        mz[2][2] = 1.0f;

        if( cl )
                matrix_matrix_mult( mz, mx,     dummy);
        else
                matrix_matrix_mult( mx, mz,     dummy);
        matrix_matrix_mult( dummy, my, m);
}

float _cubeRoot( float x ){
	if( x == 0.0f )
	{
		return 0.0f;
	}
	else if( x > 0.0f )
	{
		return (float)pow((float)x, 1.0f / 3.0f);
	}
	else
	{
		return (float)-pow((float)-x, 1.0f / 3.0f);
	}
}

void squareZero( float *a, int *n, float *root ){
	if( a[2] == 0.0 ){ // linear equation
		if( a[1] == 0.0 ){ // constant
			if( a[0] == 0.0 ){
				*n = 1; root[0] = 0.0;
			}else{
				*n = 0;
			}
		}else{
			*n = 1; root[0] = - a[0] / a[1];
		}
	}else{
		if( 4.0 * a[2] * a[0] > a[1] * a[1] ){
			*n = 0; 
		}else{
			*n = 2;
			root[0] = (float)(- a[1] + sqrt( a[1] * a[1] - 4.0f * a[2] * a[0] )) / (2.0f * a[2]);
			root[1] = (float)(- a[1] - sqrt( a[1] * a[1] - 4.0f * a[2] * a[0] )) / (2.0f * a[2]);
		}
	}
}

void cubeZero( float *a, int *n, float *root ) {
	if( a[3] == 0.0 ){ // second order polynomial
		squareZero( a, n, root );
	}else{
		float p = (float)((-1.0/3.0) * (a[2]/a[3]) * (a[2]/a[3]) + a[1]/a[3]) / 3.0f;
		float q = (float)((2.0f/27.0f) * (a[2]/a[3]) * (a[2]/a[3]) * (a[2]/a[3]) - (1.0f/3.0f) * (a[2]/a[3]) * (a[1]/a[3]) + a[0]/a[3]) / 2.0f;
		
		if( q*q + p*p*p >= 0.0f){
			*n = 1;
			root[0] = (float)cubeRoot(-q + (float)sqrt(q*q + p*p*p)) + cubeRoot(-q - (float)sqrt(q*q + p*p*p)) - a[2] / (3.0f * a[3]); 
		}else{
			float phi = (float)acos( -q / (float)sqrt(-p*p*p) );
			*n = 3;
			root[0] =  2.0f * (float)sqrt(-p) * (float)cos(phi/3.0f) - a[2] / (3.0f * a[3]); 
			root[1] = -2.0f * (float)sqrt(-p) * (float)cos(phi/3.0f + PI/3.0f) - a[2] / (3.0f * a[3]); 
			root[2] = -2.0f * (float)sqrt(-p) * (float)cos(phi/3.0f - PI/3.0f) - a[2] / (3.0f * a[3]); 
		}
	}
}

float smallestRoot( float *p ){
	int n,i;
	float root[3], sroot = 1000.0;
	
	cubeZero( p, &n, root );
	
	for( i=0; i<n; i++){
		if(root[i] > 0.0 && root[i] < sroot)
			sroot = root[i];
	}
	
	return sroot;
}

void first_step( float x_dest, float y_dest, float* x_src, float* y_src, float rot[2])
{
                *x_src = x_dest + rot[1];

                while( *x_src < - rot[0] )
                        *x_src += 2 *  rot[0];

                while( *x_src >  rot[0] )
                        *x_src -= 2 *  rot[0];

                *y_src = y_dest ;
}

void second_step( float x_dest,float  y_dest, float* x_src, float* y_src, float distanceparam)
{
	register float phi, theta, r,s;
	float v[3];

	phi 	= x_dest / distanceparam;
	theta 	=  - y_dest / distanceparam  + (float)PI / 2.0f;
	if(theta < 0)
	{
		theta = - theta;
		phi += (float)PI;
	}
	if(theta >(float)PI)
	{
		theta = (float)PI - (theta - (float)PI);
		phi += (float)PI;
	}


	s = (float)sin( theta );
	v[0] =  s * (float)sin( phi );	//  y' -> x
	v[1] = (float)cos( theta );				//  z' -> y
	
	r = (float)sqrt( v[1]*v[1] + v[0]*v[0]);

	theta = distanceparam * (float)atan2( r , s * (float)cos( phi ) );
	
	*x_src =  theta * v[0] / r;
	*y_src =  theta * v[1] / r;

}

void third_step( float x_dest,float  y_dest, float* x_src, float* y_src, void* perspect[2])
{
	register float theta,s,r;
	float v[3];

	r 		= (float)sqrt( x_dest * x_dest + y_dest * y_dest );
	theta 	=	 r / *((float*) ((void**)perspect)[1]);
	if( r == 0.0 )
		s = 0.0;
	else
		s = (float)sin( theta ) / r;

	v[0] =  s * x_dest ;
	v[1] =  s * y_dest ;
	v[2] = (float)cos( theta );

	matrix_inv_mult( (float(*)[3]) ((void**)perspect)[0], v );

	r 		= (float)sqrt( v[0]*v[0] + v[1]*v[1] );
	if( r == 0.0 )
		theta = 0.0;
	else
		theta 	= *((float*) ((void**)perspect)[1]) * (float)atan2( r, v[2] ) / r;
	*x_src 	= theta * v[0];
	*y_src 	= theta * v[1];
}	

void fourth_step( float x_dest,float  y_dest, float* x_src, float* y_src, float distanceparam)
{
	register float  theta,r,s;
	float	v[3];

	r = (float)sqrt( x_dest * x_dest + y_dest * y_dest );
	theta = r / distanceparam;
	if(theta == 0.0f)
		s = 1.0f / distanceparam;
	else
		s = (float)sin( theta) / r;
	
	v[1] =  s * x_dest;   
	v[0] = (float)cos( theta );
	

	*x_src = distanceparam * (float)atan2( v[1], v[0] );
	*y_src = distanceparam * (float)atan( s * y_dest /sqrt( v[0]*v[0] + v[1]*v[1] ) );
}

void fith_step( float x_dest, float y_dest, float* x_src, float* y_src, float scale[2])
{
	*x_src = x_dest * scale[0];
	*y_src = y_dest * scale[1];  
}

void sixth_step( float x_dest, float y_dest, float* x_src, float* y_src, float params[6])
{
	register float r, scale;

	r 		= ((float)sqrt( x_dest*x_dest + y_dest*y_dest )) / ((float*)params)[4];
	if( r < ((float*)params)[5] )
	{
		scale 	= ((((float*)params)[3] * r + ((float*)params)[2]) * r + 
				((float*)params)[1]) * r + ((float*)params)[0];
	}
	else
		scale 	= 1.0f;
		//scale = 1000.0;

	*x_src = x_dest * scale  ;
	*y_src = y_dest * scale  ;
}

void seventh_step( float x_dest, float y_dest, float* x_src, float* y_src, float shift)
{
	*x_src	= x_dest;	
	*y_src  = y_dest + shift;		
}

void height_step( float x_dest, float y_dest, float* x_src, float* y_src, float shift)
	{
	*x_src	= x_dest + shift;	
	*y_src  = y_dest;
}

pixel computeDewarping(float yaw, float pitch, float roll, pixel p)
{
	pixel toBeReturned;

	int i = 0, k = 0;
	float a_ = 0, b_ = 0; //field of view in rad
	float a = 0;
	float b = 0;
	float c = 0;
	float e = 0;
	float d = 0;
	float dd = 0;

	int image_width = 0;
	int image_height = 0;

	//float src_hfov = 0;
	float dest_width = 0;
	float dest_height = 0;
	float src_pitch = 0;
	float src_roll = 0;
	float src_yaw = 0; 
	float mt[3][3] = {0};
	float rad[6] = {0};
	float mp_distance = 0;
	float scale[2];
	float rot[2];
	void* perspect[2];
	float radial_params[3][5] = {0};
	float aa[4];
	float x_src=0, y_src=0, x_dest=0, y_dest=0;
	float x_dest2 = 0, y_dest2 = 0;
	float x_dest3 = 0, y_dest3 = 0;
	float x_dest5 = 0, y_dest5 = 0;
	float x_dest6 = 0, y_dest6 = 0;
	float x_dest8 = 0, y_dest8 = 0;
	float x_dest9 = 0, y_dest9 = 0;
	
	float x_dest2_1 = 0, y_dest2_1 = 0;

	//Initialization
	a = 0;
	b = 0;
	c = 0;

	image_width = START_WIDTH;
	image_height = START_HEIGHT;
	//src_hfov = 360.0f;
	src_yaw = yaw;
	src_pitch = pitch;
	src_roll = roll;
	dest_width = 6000;//4096;
	dest_height = 3000;//2048;
	d = 0;
	e = 0;

	a_ = (float)DEG_TO_RAD(360.0f);
	b_ = (float)DEG_TO_RAD(360.0f);

	mp_distance =  ((float) dest_width) / b_;
	SetMatrix( -(float)DEG_TO_RAD( src_pitch ), 0.0f, -(float)DEG_TO_RAD( src_roll ), mt,0);

	scale[0] = ((float) image_width) / a_ / mp_distance;
	scale[1] = scale[0];

	rot[0] = mp_distance * (float)PI;
	rot[1] = - src_yaw * mp_distance * (float)PI / 180.0f;
	 
	perspect[0] = (void*) mt; perspect[1] = (void*) & mp_distance;

    radial_params[0][3] = radial_params[1][3] = radial_params[2][3] = a;
    radial_params[0][2] = radial_params[1][2] = radial_params[2][2] = b;
    radial_params[0][1] = radial_params[1][1] = radial_params[2][1] = c;
    dd = 1.0f - (a+b+c);
    radial_params[0][0] = radial_params[1][0] = radial_params[2][0] = dd;
	
	for(i=0; i<3; i++ )
	{
		for(k=0; k<4; k++ )
		{
			aa[k] = 0.0;//1.0e-10;
			if( radial_params[i][k] != 0.0 )
			{
				aa[k] = (k+1) * radial_params[i][k];
			}
		}
		radial_params[i][4] = smallestRoot( aa );
	}

	for(i=0; i<4; i++)
	{
		rad[i]  = radial_params[0][i];
	}
	
	rad[5] = radial_params[0][4];

	rad[4]  = ( (float)( image_width < image_height ? image_width : image_height) ) / 2.0f;

	i=0;
	
	x_src = p.x * 6000/WIDTH;
	y_src = p.y * 3000/HEIGHT;

    x_src -= dest_width/2.0f - 0.5f;
    y_src -= dest_height/2.0f - 0.5f;

	first_step(x_src, y_src, &x_dest, &y_dest, rot);
	second_step(x_dest, y_dest, &x_dest2, &y_dest2, mp_distance);
	third_step(x_dest2, y_dest2, &x_dest3, &y_dest3, perspect);
	fourth_step(x_dest3, y_dest3, &x_dest2_1, &y_dest2_1, mp_distance);
	fith_step(x_dest2_1, y_dest2_1, &x_dest5, &y_dest5, scale);
	sixth_step(x_dest5, y_dest5, &x_dest6, &y_dest6, rad);
	seventh_step(x_dest6, y_dest6, &x_dest8, &y_dest8, e);
	height_step(x_dest8, y_dest8, &x_dest9, &y_dest9, d);

	x_dest9 += image_width/2.0f - 0.5f;
	y_dest9 += image_height/2.0f- 0.5f;

	toBeReturned.x = x_dest9;
	toBeReturned.y = y_dest9;

	return toBeReturned;
}