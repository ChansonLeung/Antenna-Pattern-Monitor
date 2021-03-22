const math = require('mathjs')

function sind(value){
    return math.sin(math.multiply(value, 3.1415926535/180))
}
function acos(value){
    return math.acos(math.multiply(value, 3.1415926535/180))
}
function cosd(value){
    return math.cos(math.multiply(value, 3.1415926535/180))
}
function exp(value){
    return math.exp(value)
}

//input 1d size
function get_vector(value){
    return math.transpose(math.matrix([value]))
}

var info = [4,8,8,
    1,2,3,
    0,0,0,
    100,100,100,
    0,0,
    1]
var n = 8;
var M_PI = 3.1415926535
var c = 299792458;
var freq_vector  = [info[1-1]*1e9, info[2-1]*1e9];
var fmax = freq_vector[1];
var lambda_min = c/fmax;
var fc = info[3-1]*1e9;
var lambda = c/fc;
var k = 2*M_PI/lambda;
var angle_res = 2

var x0 = info[4-1];
var y0 = info[5-1];
var z0 = info[6-1];

var pyaw = info[7-1];
var ppitch = info[8-1];
var proll = info[9-1];

var tx0 = info[10-1]*1e3;
var ty0 = info[11-1]*1e3;
var tz0 = info[12-1]*1e3;

var cyaw = info[13-1];
var cpitch = info[14-1];    

var delement = lambda_min/2;

var array_size = delement*n;

function Cal_Pattern(cyaw,cpitch,proll){
    
    //roate//
    var tx1 = ty0*sind(pyaw)+tx0*cosd(pyaw);
    var ty1 = ty0*cosd(pyaw)-tx0*sind(pyaw);
    var tz1 = tz0;
    
    var tx2 = tz1*sind(ppitch)+tx1*cosd(ppitch);
    var ty2 = ty1;
    var tz2 = tz1*cosd(ppitch)-tx1*sind(ppitch);
    
    var tx3 = tx2;
    var ty3 = tz2*sind(proll)+ty2*cosd(proll);
    var tz3 = tz2*cosd(proll)-ty2*sind(proll);
    
    var tx4 = tx3-x0;
    var ty4 = ty3-y0;
    var tz4 = tz3-z0;
    
    var tx5 = ty4*sind(cyaw)+tx4*cosd(cyaw);
    var ty5 = ty4*cosd(cyaw)-tx4*sind(cyaw);
    var tz5 = tz4;
    
    var tx6 = tz5*sind(cpitch)+tx5*cosd(cpitch);
    var ty6 = ty5;
    var tz6 = tz5*cosd(cpitch)-tx5*sind(cpitch);
    
    var cos_beta = tx6/math.pow(tx6*tx6+ty6*ty6+tz6*tz6, 0.5);
    var beta = acos(cos_beta);
    
    var in_phi = math.chain(math.matrix(  [math.multiply(math.range(0, n), -1) ]   )).multiply(k * delement * cos_beta).done();//n-1还是n?
    
    var a = math.complex(0,1);
   
    var excite = math.transpose( math.chain(a).multiply(in_phi).exp().conj().done())//值要讨论 size一维
    
    var deg_in = math.chain(in_phi).multiply(180/M_PI).done();  //size一维
    
    var x =  math.matrix( [math.range(-(n-1)*delement/2 , (n-1)*delement/2 + delement/100,delement)] ) //size一维 精度问题 ，。。加上百分之一间距
    
    var y = math.zeros(1,n);//改
    
    
    
    var az = math.transpose(math.matrix( [ math.range(-180, 180, angle_res)  ]))//180*1
    var el = math.transpose( math.matrix( [math.range(-90, 90,angle_res)])) //90*1
    
    //generate meshgrid
    //
    var azgrid_2dimension = math.chain(math.transpose(math.matrix( [math.ones( el.size()[0]) ]))  ).multiply(math.transpose(az)).done()//90*180
  
    var elgrid_2dimension = math.chain(el).multiply(  math.matrix( [math.ones(az.size()[0])]) ).done() //90*180
    

    var azgrid_1dimension = math.reshape(math.transpose(  Object.create(azgrid_2dimension)), [az.size()[0]*el.size()[0] ,1])//16200*1
    var elgrid_1dimension = math.reshape( math.transpose( Object.create(elgrid_2dimension)), [az.size()[0]*el.size()[0] ,1])//16200*1

    var u_l = math.chain(elgrid_1dimension).multiply(M_PI/180) .cos().done()
    var u_r = math.chain(azgrid_1dimension).multiply(M_PI/180) .cos().done() 

    var u = math.dotMultiply(math.chain(elgrid_1dimension).multiply(M_PI/180).cos().done() ,  math.chain(azgrid_1dimension).multiply(M_PI/180).cos().done()  )
    var v = math.dotMultiply(math.chain(elgrid_1dimension).multiply(M_PI/180).cos().done() ,  math.chain(azgrid_1dimension).multiply(M_PI/180).sin().done()   )
    
    //main time cost!!! over 3s
    
    //size row:el.size*az.size    column: array.size
    var bn = math.chain(u).multiply(x).add(math.multiply(v, y)  ).multiply(-k).multiply(a).exp().done()
    var fn = math.ones(u.size()[0],1)
    
    var Matrix_x = bn;
    
    //main time cost!!! over 1.2
    
    var f = math.chain(bn).multiply(excite).abs().done()
    
    
    //Matrix of row:el.size    column:az.size
    var f_reshape = math.transpose(math.reshape( Object.create(f), ([az.size()[0] ,el.size()[0]]) ))
    
    
    var el2 = math.add(elgrid_2dimension, cpitch)
    var az2 = math.add(azgrid_2dimension, cyaw)

    return{
        xx: (math.dotMultiply( math.dotMultiply(f_reshape, cosd(el2)), cosd(az2) )),
        yy: (math.dotMultiply( math.dotMultiply(f_reshape, cosd(el2)), sind(az2) )),
        zz: (math.dotMultiply(f_reshape, sind(el2)))
    }
}
export{
    Cal_Pattern
}