// // hello.cc
// #include <node.h>
// #include <iostream>
// #include "./Eigen/Dense"
// #include "math.h"
// #include <complex>
// #include "nan.h"
// using namespace Eigen;
// namespace eigen {
// using v8::FunctionCallbackInfo;
// using v8::Context;
// using v8::Isolate;
// using v8::Isolate;
// using v8::Local;
// using v8::Object;
// using v8::String;
// using v8::Value;
// using v8::Array;
// using Eigen::MatrixXf;
// using Eigen::MatrixXcf;
// using Eigen::VectorXf;
// using Eigen::VectorXcf;
// using std::complex;


// typedef MatrixXf MatrixXType ;
// typedef MatrixXcf MatrixXcType;
// typedef VectorXf VectorXType;
// typedef VectorXcf VectorXcType;
// #define sind(x) (sin(fmod((x),360) * M_PI / 180))
// #define cosd(x) (cos(fmod((x),360) * M_PI / 180))
// #define M_PI   (3.1415926535)
// void Method(const FunctionCallbackInfo<Value>& args) {

// Isolate* isolate = args.GetIsolate();

// //config
// double angle_res = 2;

// //input
// double info[15] = {4,8,8,
//        1,2,3,
//        20,45,30,
//        100,200,100,
//        -30,-15,
//        1};

// int n = 8;



//   double c = 299792458;
// double freq_vector[2]  = {info[1-1]*1e9, info[2-1]*1e9};
// double fmax = freq_vector[1];
// double lambda_min = c/fmax;
// double fc = info[3-1]*1e9;
// double lambda = c/fc;
// double k = 2*M_PI/lambda;

// double x0 = info[4-1];
// double y0 = info[5-1];
// double z0 = info[6-1];

// double pyaw = info[7-1];
// double ppitch = info[8-1];
// double proll = info[9-1];

// double tx0 = info[10-1]*1e3;
// double ty0 = info[11-1]*1e3;
// double tz0 = info[12-1]*1e3;

// double cyaw = info[13-1];
// double cpitch = info[14-1];

// double delement = lambda_min/2;

// double array_size = delement*n;

// //roate//
// double tx1 = ty0*sind(pyaw)+tx0*cosd(pyaw);
// double ty1 = ty0*cosd(pyaw)-tx0*sind(pyaw);
// double tz1 = tz0;

// double tx2 = tz1*sind(ppitch)+tx1*cosd(ppitch);
// double ty2 = ty1;
// double tz2 = tz1*cosd(ppitch)-tx1*sind(ppitch);

// double tx3 = tx2;
// double ty3 = tz2*sind(proll)+ty2*cosd(proll);
// double tz3 = tz2*cosd(proll)-ty2*sind(proll);

// double tx4 = tx3-x0;
// double ty4 = ty3-y0;
// double tz4 = tz3-z0;

// double tx5 = ty4*sind(cyaw)+tx4*cosd(cyaw);
// double ty5 = ty4*cosd(cyaw)-tx4*sind(cyaw);
// double tz5 = tz4;

// double tx6 = tz5*sind(cpitch)+tx5*cosd(cpitch);
// double ty6 = ty5;
// double tz6 = tz5*cosd(cpitch)-tx5*sind(cpitch);

// double cos_beta = tx6/pow(tx6*tx6+ty6*ty6+tz6*tz6, 0.5);
// double beta = acos(cos_beta);

// VectorXType in_phi = -VectorXType::LinSpaced(n, 0, n-1) * k * delement * cos_beta;

// complex<float> a;
// a.real(0);
// a.imag(1);

// VectorXcType excite = (a *in_phi).array().exp().conjugate();



// VectorXType deg_in = in_phi*180/M_PI;

// VectorXType x = VectorXType::LinSpaced(n, -(n-1)*delement/2, (n-1)*delement/2);

// VectorXType y = VectorXType::Zero(n);



// static VectorXType az = VectorXType::LinSpaced(360/angle_res+1,-180, 180);
// static VectorXType el = VectorXType::LinSpaced(180/angle_res+1,-90, 90);

// //generate meshgrid
// static MatrixXType azgrid_2dimension = VectorXType::Ones(el.size())*az.transpose();

// static MatrixXType 	elgrid_2dimension = el*VectorXType::Ones(az.size()).transpose();

// Map<MatrixXType> 	azgrid_1dimension(azgrid_2dimension.data(), el.size()*az.size(),1 );
// Map<MatrixXType> 	elgrid_1dimension(elgrid_2dimension.data(), el.size()*az.size(),1 );


// static VectorXType u = ((elgrid_1dimension.array()*M_PI/180).array().cos()) * (azgrid_1dimension.array()*M_PI/180).cos();

// static VectorXType v = (elgrid_1dimension.array()*M_PI/180).array().cos()  *  (azgrid_1dimension.array()*M_PI/180).sin();

// //main time cost!!! over 3s

// //size row:el.size*az.size    column:array.size
// MatrixXcType bn = ((u*x.transpose()+v*y.transpose())*(-a)*k).array().exp();


// VectorXType fn = VectorXType::Ones(u.size());

// MatrixXcType Matrix_x = bn;

// //main time cost!!! over 1.2

// MatrixXType f = (bn*excite).array().abs();


// //Matrix of row:el.size    column:az.size
// Map<MatrixXType> f_reshape(f.data(), el.size(), az.size());


// MatrixXType el2 = elgrid_2dimension.array() + cpitch;
// MatrixXType az2 = azgrid_2dimension.array() + cyaw  ;


// MatrixXType xx = f_reshape.array()*  (el2.array()*M_PI/180).cos()*(az2.array()*M_PI/180).cos();
// MatrixXType yy = f_reshape.array()* (el2.array()*M_PI/180).cos()*(az2.array()*M_PI/180).sin();
// MatrixXType zz = f_reshape.array()* (el2.array()*M_PI/180).sin();

//   // args.GetReturnValue().Set(String::NewFromUtf8(
//   //     isolate, "Hello").ToLocalChecked());

//   Local<Context> context = isolate->GetCurrentContext();
//   Local<Object> obj = Object::New(isolate);
//   Local<Array> myArray = Array::New(isolate);

//   for (int i = 0; i < n; i++) {
//     Local<Object> obj = Object::New(isolate);
//     obj->Set(context, String::NewFromUtf8(isolate, "tag1"), "test");
//     myArray->Set(context, i, obj);
//  }

//   args.GetReturnValue().Set(myArray);




// }

// void Init(Local<Object> exports, Local<Object> module) {
//   NODE_SET_METHOD(exports, "hello",Method);
// }

// NODE_MODULE(addon, Init)

// }  // 命名空间示例










#include <nan.h>
#include "./Eigen/Dense"
using Eigen::VectorXf;
using Eigen::VectorXcf;
typedef VectorXf VectorXType;
typedef VectorXcf VectorXcType;
using namespace Eigen;
typedef MatrixXf MatrixXType;
typedef MatrixXcf MatrixXcType;
typedef VectorXf VectorXType;
typedef VectorXcf VectorXcType;
#define sind(x) (sin(fmod((x),360) * M_PI / 180))
#define cosd(x) (cos(fmod((x),360) * M_PI / 180))
#define M_PI   (3.1415926535)
using Eigen::MatrixXf;
using Eigen::MatrixXcf;
using Eigen::VectorXf;
using Eigen::VectorXcf;
using std::complex;

void CreateObject(const Nan::FunctionCallbackInfo<v8::Value>& infoo) {
  v8::Local<v8::Context> context = infoo.GetIsolate()->GetCurrentContext();
  


  
//config
double angle_res = 2;

//input
double info[15] = {4,8,8,
       1,2,3,
       20,45,30,
       100,200,100,
       -30,-15,
       1};

int n = 8;



  double c = 299792458;
double freq_vector[2]  = {info[1-1]*1e9, info[2-1]*1e9};
double fmax = freq_vector[1];
double lambda_min = c/fmax;
double fc = info[3-1]*1e9;
double lambda = c/fc;
double k = 2*M_PI/lambda;

double x0 = info[4-1];
double y0 = info[5-1];
double z0 = info[6-1];

double pyaw = info[7-1];
double ppitch = info[8-1];
double proll = info[9-1];

double tx0 = info[10-1]*1e3;
double ty0 = info[11-1]*1e3;
double tz0 = info[12-1]*1e3;

double cyaw = info[13-1];
double cpitch = info[14-1];

double delement = lambda_min/2;

double array_size = delement*n;

//roate//
double tx1 = ty0*sind(pyaw)+tx0*cosd(pyaw);
double ty1 = ty0*cosd(pyaw)-tx0*sind(pyaw);
double tz1 = tz0;

double tx2 = tz1*sind(ppitch)+tx1*cosd(ppitch);
double ty2 = ty1;
double tz2 = tz1*cosd(ppitch)-tx1*sind(ppitch);

double tx3 = tx2;
double ty3 = tz2*sind(proll)+ty2*cosd(proll);
double tz3 = tz2*cosd(proll)-ty2*sind(proll);

double tx4 = tx3-x0;
double ty4 = ty3-y0;
double tz4 = tz3-z0;

double tx5 = ty4*sind(cyaw)+tx4*cosd(cyaw);
double ty5 = ty4*cosd(cyaw)-tx4*sind(cyaw);
double tz5 = tz4;

double tx6 = tz5*sind(cpitch)+tx5*cosd(cpitch);
double ty6 = ty5;
double tz6 = tz5*cosd(cpitch)-tx5*sind(cpitch);

double cos_beta = tx6/pow(tx6*tx6+ty6*ty6+tz6*tz6, 0.5);
double beta = acos(cos_beta);

VectorXType in_phi = -VectorXType::LinSpaced(n, 0, n-1) * k * delement * cos_beta;

complex<float> a;
a.real(0);
a.imag(1);

VectorXcType excite = (a *in_phi).array().exp().conjugate();



VectorXType deg_in = in_phi*180/M_PI;

VectorXType x = VectorXType::LinSpaced(n, -(n-1)*delement/2, (n-1)*delement/2);

VectorXType y = VectorXType::Zero(n);



static VectorXType az = VectorXType::LinSpaced(360/angle_res+1,-180, 180);
static VectorXType el = VectorXType::LinSpaced(180/angle_res+1,-90, 90);

//generate meshgrid
static MatrixXType azgrid_2dimension = VectorXType::Ones(el.size())*az.transpose();

static MatrixXType 	elgrid_2dimension = el*VectorXType::Ones(az.size()).transpose();

Map<MatrixXType> 	azgrid_1dimension(azgrid_2dimension.data(), el.size()*az.size(),1 );
Map<MatrixXType> 	elgrid_1dimension(elgrid_2dimension.data(), el.size()*az.size(),1 );


static VectorXType u = ((elgrid_1dimension.array()*M_PI/180).array().cos()) * (azgrid_1dimension.array()*M_PI/180).cos();

static VectorXType v = (elgrid_1dimension.array()*M_PI/180).array().cos()  *  (azgrid_1dimension.array()*M_PI/180).sin();

//main time cost!!! over 3s

//size row:el.size*az.size    column:array.size
MatrixXcType bn = ((u*x.transpose()+v*y.transpose())*(-a)*k).array().exp();


VectorXType fn = VectorXType::Ones(u.size());

MatrixXcType Matrix_x = bn;

//main time cost!!! over 1.2

MatrixXType f = (bn*excite).array().abs();


//Matrix of row:el.size    column:az.size
Map<MatrixXType> f_reshape(f.data(), el.size(), az.size());


MatrixXType el2 = elgrid_2dimension.array() + cpitch;
MatrixXType az2 = azgrid_2dimension.array() + cyaw  ;


MatrixXType xx = f_reshape.array()*  (el2.array()*M_PI/180).cos()*(az2.array()*M_PI/180).cos();
MatrixXType yy = f_reshape.array()* (el2.array()*M_PI/180).cos()*(az2.array()*M_PI/180).sin();
MatrixXType zz = f_reshape.array()* (el2.array()*M_PI/180).sin();

v8::Local<v8::Object> obj = Nan::New<v8::Object>();
v8::Local<v8::Array> result_x = Nan::New <v8::Array>(16200);
v8::Local<v8::Array> result_y = Nan::New <v8::Array>(16200);
v8::Local<v8::Array> result_z = Nan::New <v8::Array>(16200);

for(int i = 0; i<16200; i++){
  Nan::Set(result_x, i, Nan::New(xx.data()[i]));
  Nan::Set(result_y, i, Nan::New(yy.data()[i]));
  Nan::Set(result_z, i, Nan::New(zz.data()[i]));
}

  obj->Set(context,
           Nan::New("xx").ToLocalChecked(),
           result_x );
  obj->Set(context,
           Nan::New("yy").ToLocalChecked(),
           result_y );
  obj->Set(context,
           Nan::New("zz").ToLocalChecked(),
           result_z );

  infoo.GetReturnValue().Set(obj);
}

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  v8::Local<v8::Context> context = exports->CreationContext();
  module->Set(context,
              Nan::New("exports").ToLocalChecked(),
              Nan::New<v8::FunctionTemplate>(CreateObject)
                  ->GetFunction(context)
                  .ToLocalChecked());
}

NODE_MODULE(addon, Init)