/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "meshkit.h"
#include "orbkit.h"
#include "sigkit.h"
#include "AcTypes.h"

#ifndef __42TYPES_H__
#define __42TYPES_H__

/*
** #ifdef __cplusplus
** namespace _42 {
** #endif
*/


struct SphereHarmType {
   /*~ Internal Variables ~*/
   long Type;
   long N;
   long M;
};

struct FormationType {
   /*~ Internal Variables ~*/
   char FixedInFrame;
   double CN[3][3];
   double CL[3][3];
   double PosR[3]; /* Position of F wrt R, expressed in N */
};

/* "Analysis" nodes, used both for Flex, ("Force" nodes and "Measurement" nodes) */
/* and for more general purposes (sensor, actuator positions) */
struct NodeType {
   /*~ Internal Variables ~*/
   char comment[80]; 
   double NomPosB[3];
   double PosCm[3]; /* Pos wrt B's cm, expressed in B */
   double **PSI;  /* Translational Mode shapes, 3 x B.Nf */
   double **THETA; /* Rotational Mode shapes, 3 x B.Nf */
   double Frc[3];  /* Expressed in B */
   double Trq[3]; /* Expressed in B */
   double *FlexFrc;  /* "Fbendy + Tbendy", B.Nf x 1 [*Nf*] */
   double FlexPos[3];
   double FlexVel[3];
   double FlexAng[3];
   double FlexAngRate[3]; /* Deflection variables */
   double PosB[3];
   double VelB[3];
   double VelN[3];
   double qb[4];
   double AngVelB[3];
   long ContactPoly; /* Remember Region poly for contact constraint */
};

struct ShakerType {
   /*~ Table Parameters ~*/
   long Body;
   long Node;
   long FrcTrq;
   double Axis[3];
   long Ntone;
   long RandomActive;
   double *ToneAmp; /* N or Nm [*Ntone*] */
   double *ToneFreq; /* For tonic, rad/sec [*Ntone*] */
   double *TonePhase; /* For tonic, rad [*Ntone*] */
   double LowBandLimit; /* rad/sec */
   double HighBandLimit; /* rad/sec */
   double RandStd; /* Std Dev of band-limited random input, N or Nm */
   
   /*~ Outputs ~*/
   double Output;

   /*~ Internal Variables ~*/

   /*~ Structures ~*/
   struct RandomProcessType *RandomProc;
   struct FilterType *Lowpass;
   struct FilterType *Highpass;
   struct FilterType *Rand; /* White noise in, band-limited noise out */
};

struct BodyType {
   /*~ Outputs ~*/
   double wn[3]; /* Angular Velocity of B wrt N expressed in B frame [[rad/sec]] [~=~] */
   double qn[4]; /* [~=~] */

   /*~ Internal Variables ~*/
   double mass;
   double cm[3]; /* wrt origin of convenience, expressed in B frame */
   double c[3]; /* First mass moment about ref pt, expressed in B */
   double I[3][3]; /* Moment of Inertia, about ref pt, expressed in B frame */ 
   double EmbeddedMom[3]; /* Constant embedded momentum, for CMGs and rotating instruments */
   double EmbeddedDipole[3]; /* Constant embedded magnetic moment [[A-m^2]] */
   double vn[3]; /* velocity of B ref pt expressed in N frame */
   double pn[3]; /* position of B ref pt in N frame expressed in N frame */
   double CN[3][3]; /* Direction Cosine of B frame in N frame */
   double Trq[3]; /* expressed in B */
   double FrcN[3]; /* expressed in N */
   double alpha[3]; /* Angular acceleration of B wrt N, expressed in B */
   double accel[3]; /* Linear acceleration of B wrt N, expressed in N */
   char MeshFileName[40];
   char NodeFileName[40];
   char FlexFileName[40];
   float ModelMatrix[16]; /* For OpenGL */
   long MeshTag;
   /* For KaneNBody Dynamics */
   long Gin;  /* Joint that B is Bout of */
   double beta[3];  /* Vector from B ref pt to B[0] ref pt, expressed in N */
   double AlphaR[3];  /* Remainder alpha, expressed in B */
   double AccR[3];    /* Remainder acc, expressed in N */
   double InertiaTrq[3];
   double InertiaFrc[3];
   double JointTrq[3]; /* From all joints exerting trq on this body */
   
   /* For OrderN Dynamics */
   long Nd; /* Number of distal joints (i.e. for which this body is Bi) */
   long *Gd; /* Indices of distal joints (i.e. for which this body is Bi) [*Nd*] */
   double RemAlf[3];
   double RemAcc[3];
   double alfn[3];
   double accn[3];
   double H[3];
   double RemInertiaFrc[6];  
   double WhlMom[3];
   double FrcB[3]; /* Expressed in B */
   double SpatFrc[6]; /* [Trq;Frc] + [PassiveTrq;PassiveFrc] */
   
   double AccU[6];

   /* For Flex Formulation */
   long Nf;  /* Number of flex modes superimposed on this body */
   double *xi;    /* Flex speed coordinate, Nf x 1 [*Nf*] */
   double *eta;   /* Flex position coordinate, Nf x 1 */
   double **Mf;   /* Flex Mass Matrix, Nf x Nf */
   double **Kf;   /* Flex Stiffness Matrix, Nf x Nf */
   double **Cf;   /* Flex Damping Matrix, Nf x Nf */
   double **Pf;   /* Flex tensor, 3 x Nf */
   double **Hf;   /* Flex tensor, 3 x Nf */
   double *Qf;  /* Flex tensor, 3 x Nf x Nf */
   double *Rf;  /* Flex tensor, 3 x Nf x 3 */
   double *Sf; /* Flex tensor, 3 x Nf x Nf x 3 */
   long f0;      /* Index of first element in uf */
   double Peta[3]; /* Pf*eta */
   double cplusPeta[3][3]; /* SkewMatrix of (c + Pf*eta) */
   double **CnbP; /* CNB*Pf, 3 x Nf */
   double **HplusQeta; /* Hf + Qf*eta, 3 x Nf */
   double **Qxi;  /* Qf*xi, 3 x Nf */
   double **Rw;   /* Rf*w, 3 x Nf */
   double *Sw;  /* Sf*w, 3 x Nf * Nf */
   double **Swe;  /* Sf*w*eta, 3 x Nf */
   long NumNodes;  /* Number of flex "analysis" nodes on Body */
   long MfIsDiagonal;  /* Simpler EOM for One-body case if Mf is diagonal */
   
   /*~ Structures ~*/   
   struct NodeType *Node; /* [*NumNodes*] */
};

struct JointType {
   /*~ Outputs ~*/
   double Pos[3];         /* translational kinematic state variables [~=~] */
   double PosRate[3];     /* translational dynamic state variables [~=~] */
   double Ang[3];         /* Joint Euler angles [~=~] */
   double AngRate[3];     /* Euler angle rates about gim axes [~=~] */
   
   /*~ Internal Variables ~*/
   long Type;  /* PASSIVE_JOINT, ACTUATED_JOINT, etc */ 
   long Init;
   long IsSpherical;      /* TRUE or FALSE */
   long RotDOF;           /* 0,1,2,3 */
   long TrnDOF;           /* 0,1,2,3 */
   long Bin;              /* Index of inner body */
   long Bout;             /* Index of outer body */
   struct BodyType *Bi;
   struct BodyType *Bo;
   long Nanc;             /* Number of "ancestor" joints: joints between this one and B[0] */
   long *Anc;             /* Indices of ancestor joints */
   double RigidRin[3];    /* Position wrt inner body ref pt (rigid) */
   double RigidRout[3];   /* Position wrt outer body ref pt (rigid) */
   double ri[3];         /* Position wrt inner body ref pt (incl flex & TrnDOF) */
   double ro[3];        /* Position wrt outer body ref pt (incl flex) */
   long RotSeq;           /* Joint Euler sequence */
   long RotLocked[3];     /* Set TRUE if individual DOF is to be locked in place */
   long TrnSeq;           /* Translational joint sequence */
   long TrnLocked[3];
   double xb[3];          /* translational displacement in the Bi frame */
   double xn[3];          /* translational displacement in the N frame */
   double AngRateCmd[3];     /* Euler angle rate commands, rad/sec */
   double PosRateCmd[3];     /* Translation rate commands, m/sec */
   double AngRateGain[3];
   double PosRateGain[3];
   double MaxAngRate[3];
   double MaxPosRate[3];
   double MaxTrq[3];
   double MaxFrc[3];
   double RotSpringCoef[3];  /* For passive joint torques */
   double RotDampCoef[3];    /* For passive joint torques */
   double TrnSpringCoef[3];  /* For passive joint forces */
   double TrnDampCoef[3];    /* For passive joint forces */
   /* Frames involved in a joint: Bo <-> (Bfo) <-> Go <-> Gi <-> (Bfi) <-> Bi */
   double CGiBi[3][3];    /* Constant orientation of joint in Bi (Bfi) */
   double CBoGo[3][3];    /* Constant orientation of joint in Bo (Bfo) */
   double CGoGi[3][3];    /* Euler rot through ang[0], ang[1], ang[2] */
   double CTrqBo[3][3];   /* Used for transforming joint torques or forces from Go to Bo */
   double CTrqBi[3][3];   /* Used for transforming joint torques from Go to Bi */
   double COI[3][3];      /* DCM from inner body to outer body, (incl flex) */
   double Trq[3];         /* Exerted on Bout, components along gimbal axes */
   double Frc[3];         /* Force exerted on Bout, components along translational axes */
   double Gamma[3][3];    /* w = Gamma*sigma */
   double Delta[3][3];    /* v = Delta*s -- matrix of joint partials for translational joints */
   double Gs[3];          /* Gamma*sigma */
   double Gds[3];         /* Gammadot*sigma */
   double Ds[3];          /* Delta*s */
   double Dds[3];         /* Deltadot*s */
   long Rotu0;            /* Index of first Rot element in u */
   long Rotx0;            /* Index of first Rot element in x */
   long Trnu0;            /* Index of first Trn element in u */
   long Trnx0;            /* Index of first Trn element in x */
   long ActiveRotu0;         /* Index in DynStateIdx of first unlocked Rot DOF in DynState */
   long ActiveTrnu0;         /* Index in DynStateIdx of first unlocked Trn DOF in DynState */
   long ActiveRotDOF;        /* Number of unlocked RotDOF */
   long ActiveTrnDOF;        /* Number of unlocked TrnDOF */

   /* For OrderN Dynamics */
   double Pw[3][3];
   double Pv[3][3];
   double Pwdot[3][3];
   double P[6][6]; /* [Pw 0; 0 Pv] */
   double ArtFrc[6]; /* Articulated Body Force */
   double ArtMass[6][6]; /* Articulated Body Mass */
   double DynMtx[6][6]; /* Nu x Nu */
   double InvDynMtx[6][6]; /* Nu x Nu */
   double InvDynPT[6][6]; /* Nu x 6 */
   double AbsorpMtx[6][6];
   double TransMtx[6][6];
   double riplusPx[3];  /* r_{ik} + P_{vk}x_k */
   long Nu; /* RotDOF + TrnDOF */
   double q[4]; /* Quaternion or angle states, depending on IsSpherical */
   double udot[6];
   double qdot[4];
   double xdot[3];
   double RKum[6]; 
   double RKqm[4];
   double RKxm[3];
   double RKdu[6];
   double RKdq[4];
   double RKdx[3];   
   
   /* For Flex */
   double **PSIi;         /* Translation Mode Shapes, 3 x Bi.Nf */
   double **THETAi;       /* Rotational Mode Shapes, 3 x Bi.Nf */
   double **PSIo;         /* Translation Mode Shapes, 3 x Bo.Nf */
   double **THETAo;       /* Rotational Mode Shapes, 3 x Bo.Nf */
   double FlexPosi[3];    /* Translational Flex Deflection of Bi at G (d) */
   double FlexVeli[3];    /* Translational Flex Velocity of Bi at G   (e) */
   double FlexAngi[3];    /* Rotational Flex Deflection of Bi at G    (delta) */
   double FlexAngVeli[3]; /* Rotational Flex Velocity of Bi at G      (eta) */
   double FlexPoso[3];    /* Translational Flex Deflection of Bo at G (d) */
   double FlexVelo[3];    /* Translational Flex Velocity of Bo at G   (e) */
   double FlexAngo[3];    /* Rotational Flex Deflection of Bo at G    (delta) */
   double FlexAngVelo[3]; /* Rotational Flex Velocity of Bo at G      (eta) */
   /* For Constraints */
   long Rotc0;
   long Trnc0;
   
   char ParmFileName[40];
};

struct IdealActType {
   /*~ Internal Variables ~*/
   double Tcmd;
   double Fcmd;
   
   struct DelayType *FrcDelay;
   struct DelayType *TrqDelay;
};

struct WhlHarmType {
   double n;  /* Harmonic Number [none] */
   double Ks;  /* Static imbalance coefficient, [kg-m] */
   double Kd;  /* Dynamic imbalance coefficient, [kg-m^2] */
   double phase; /* Phase angle of harmonic wrt dynamic imbalance, [rad] */
};

struct WhlType {
   /*~ Outputs ~*/
   double H;  /* Angular Momentum, [[Nms]] [~=~] */
   
   /*~ Internal Variables ~*/
   long Body; /* Body that wheel is mounted in */
   double J;  /* Rotary inertia, kg-m^2 */
   double w;  /* Angular speed, rad/sec */
   double Ang; /* Spin phase angle, rad */
   double A[3]; /* Axis vector wrt Body */
   double Uaxis[3]; /* Transverse axes */
   double Vaxis[3]; /* Transverse axes */
   double Tmax;
   double Hmax;
   double Tcmd;
   double Trq;  /* Exerted on wheel, expressed along wheel axis */
   long Node;
   struct DelayType *Delay; /* For injecting delay into control loops */
   
   char DragJitterFileName[40];

   /* For Drag */
   double CoulCoef; /* Coulomb friction, Nm */
   double StribeckCoef; /* Stiction - Coulomb, Nm */
   double ViscCoef; /* Viscous friction coefficient, Nm/(rad/sec) */
   double StribeckZone; /* Stribeck zone, rad/sec */
   double LugreSpringCoef; /* Lugre stiffness, Nm/rad */
   double LugreDampCoef; /* Lugre damping, Nm/(rad/sec) */
   double LugreDampZone; /* Lugre damping zone, rad/sec */
   double z; /* Lugre internal state, rad */
   double FricTrq; /* Friction Torque, Nm */

   /* For Jitter */
   double gamma; /* 2*Jt/Jr (<1.0) */
   double Jt; /* Transverse rotor inertia, [kg-m^2] */
   double ImbPhase; /* Phase of static imbalance wrt dynamic imbalance [rad] */
   double LatFreq; /* [rad/sec] */
   double LatDamp;
   double RockFreq; /* [rad/sec] */
   double RockDamp;
   long NumHarm;
   struct WhlHarmType *Harm;
   double JitFrc[3];
   double JitTrq[3];

   /* For OrderN Dynamics */
   double Hdot;
   double RKHm;
   double RKdH;
};

struct MTBType {
   /*~ Internal Variables ~*/
   double M;
   double A[3]; /* Axis vector wrt Body 0 */
   double Mmax;
   double Mcmd;
   double Trq[3]; /* Exerted on Body 0, expressed in B[0] frame */
   long Node;
   struct DelayType *Delay; /* For injecting delay into control loops */
};

struct ThrType {
   /*~ Internal Variables ~*/
   long Mode; /* THR_PULSED or THR_PROPORTIONAL */
   double Fmax;
   double F;
   long Body; /* Body that thruster is mounted on */
   long Node;
   double A[3]; /* Axis vector wrt Body 0 */
   double PulseWidthCmd;  /* [[sec]], for THR_PULSED */
   double ThrustLevelCmd; /* [{0.0:1.0}], for THR_PROPORTIONAL */
   double Frc[3]; /* Force exerted */
   double Trq[3]; /* Torque exerted */
   struct DelayType *Delay; /* For injecting delay into control loops */
};

struct GyroType {
   /*~ Parameters ~*/
   double SampleTime;
   long MaxCounter;
   double Axis[3];
   double MaxRate;
   double Scale;
   double Quant;
   double SigV; /* ARW, rad/rt-sec */
   double SigU; /* Bias Stability, rad/sec^1.5 */
   double SigE; /* Angle Readout Noise, rad */
   long Node;
   
   double BiasStabCoef;
   double ARWCoef;
   double AngNoiseCoef;
   double CorrCoef; /* Correlation Coef, exp(-SampleTime/BiasTime) */

   /*~ Outputs ~*/
   double TrueRate; /* rad/sec [~>~] */

   /*~ Internal Variables ~*/
   long SampleCounter;
   double Bias; /* rad/sec */
   double Angle; /* rad */
   double MeasRate; /* rad/sec */
};

struct MagnetometerType {
   /*~ Parameters ~*/
   double SampleTime;
   long MaxCounter;
   double Axis[3];
   double Saturation;
   double Scale;
   double Quant;
   double Noise;
   long Node;

   /*~ Outputs ~*/
   double Field; /* Magfield Component, Tesla [~>~] */

   /*~ Internal Variables ~*/
   long SampleCounter;
};

struct CssType {
   /*~ Parameters ~*/
   double SampleTime;
   long MaxCounter;
   long Body;
   double Axis[3];
   double FovHalfAng;
   double CosFov;
   double Scale;
   double Quant;
   long Node;

   /*~ Outputs ~*/
   long Valid; /* [~>~] */
   double Illum; /* Units defined by scale [~>~] */

   /*~ Internal Variables ~*/
   long SampleCounter;
   double Albedo; /* [0.0:1.0] */
};

struct FssType {
   /*~ Parameters ~*/
   double SampleTime;
   long MaxCounter;
   double qb[4];
   double CB[3][3];
   double FovHalfAng[2];
   double NEA;
   double Quant;
   long Node;
   long BoreAxis; /* X_AXIS, Y_AXIS, Z_AXIS */
   long H_Axis; /* (BoreAxis+1)%3 */
   long V_Axis; /* (BoreAxis+2)%3 */

   /*~ Outputs ~*/
   long Valid; /* [~>~] */
   double SunAng[2]; /* [~>~] */

   /*~ Internal Variables ~*/
   long SampleCounter;
   double SunVecS[3];
   double SunVecB[3];
   double AlbA;
   double AlbB;
   double AlbC;
   double AlbD;
};

struct StarTrackerType {
   /*~ Parameters ~*/
   double SampleTime;
   long MaxCounter;
   double qb[4];
   double CB[3][3];
   double FovHalfAng[2];
   double CosFov[2];
   double SunExclAng;
   double CosSunExclAng;
   double EarthExclAng;
   double CosEarthExclAng;
   double MoonExclAng;
   double CosMoonExclAng;
   double NEA[3];
   long Node;
   long BoreAxis; /* X_AXIS, Y_AXIS, Z_AXIS */
   long H_Axis; /* (BoreAxis+1)%3 */
   long V_Axis; /* (BoreAxis+2)%3 */

   /*~ Outputs ~*/
   long Valid; /* [~>~] */
   double qn[4]; /* [~>~] */

   /*~ Internal Variables ~*/
   long SampleCounter;
};

struct GpsType {
   /*~ Parameters ~*/
   double SampleTime;
   long MaxCounter;
   double PosNoise;
   double VelNoise;
   double TimeNoise;
   long Node;
   
   /*~ Outputs ~*/
   long Valid; /* [~>~] */
   long Rollover; /* [~>~] */
   long Week; /* [~>~] */
   double Sec; /* [~>~] */
   double PosN[3]; /* [~>~] */
   double VelN[3]; /* [~>~] */
   double PosW[3]; /* [~>~] */
   double VelW[3]; /* [~>~] */

   /*~ Internal Variables ~*/
   long SampleCounter;
   double Lng,Lat,Alt; /* Geocentric */
   double WgsLng,WgsLat,WgsAlt; /* Geodetic, WGS-84 */
};

struct AccelType {
   /*~ Parameters ~*/
   double SampleTime;
   long SampleCounter;
   long MaxCounter;
   long Node;
   double Axis[3]; /* Mounting matrix */
   double Quant;
   double Scale;
   double SigV; /* DVRW m/s/rt-sec */
   double SigU;/* Bias Stability m/s^1.5 */
   double SigE; /* DV Readout Noise, m/s  */
   
   /*~ Outputs ~*/
   double TrueAcc; /* m/s^2 [~>~] */

   /*~ Internal Variables ~*/
   double AccumAccN[3];
   double Bias; /* m/s^2 */
   double PrevVelN[3]; /* m/s */
   double PrevQN[4];
   double DV; /* Change in velocity m/s */
   double MeasAcc; /* m/s^2 */
   double MaxAcc; /* m/s^2 max acceleration */
   double AccError;
   long Counts;

   /* Coef */
   double BiasStabCoef;
   double DVRWCoef;
   double DVNoiseCoef; 
   double CorrCoef; /* Correlation Coef, exp(-SampleTime/BiasTime) */
};

struct OpticsType {
   long SC;
   long Body;
   long Node;
   long Type;
   double Axis[3];
   double FocLen;
   double ConicConst;
   double ConicSign;   
   double ApRad;
};

struct GuideWindowType {
   /*~ Parameters ~*/
   long Nrow;
   long Ncol;

   /*~ Internal Variables ~*/
   long Row0;
   long Col0;
   double *Image; /* Nrow x Ncol, grayscale */
};

struct PsfType {
   long Nrow;
   long Ncol;
   long BytesPerPixel;
   double Scl; /* rad/pixel */
   double *Image;
};

struct FgsType {
   /*~ Parameters ~*/
   long HasOptics;
   double SampleTime;
   long MaxCounter;
   double qb[4];
   double CB[3][3];
   double qr[4]; /* q_fr_r */
   double CR[3][3]; /* CFrR */
   double NEA;
   long Body;
   long Node;
   long BoreAxis; /* X_AXIS, Y_AXIS, Z_AXIS */
   long H_Axis; /* (BoreAxis+1)%3 */
   long V_Axis; /* (BoreAxis+2)%3 */
   double FovHalfAng[2];
   double Scl; /* rad/pixel */
   double Hr; /* Guide Star in Fr */
   double Vr; /* Guide Star in Fr */

   /*~ Internal Variables ~*/
   long SampleCounter;
   long Valid;
   double StarVecR[3];
   double H; 
   double V;
   double Ang[3];
   char OpticsFileName[40];
   char PsfFileName[40];
   
   long Nopt;
   struct OpticsType *Opt;
   long ApFocus;
   long DetFocus;

   struct PsfType PSF;   
   struct GuideWindowType Gw;
};

struct JointPathTableType { /* tells if joint is in path of body*/
   /*~ Internal Variables ~*/
   long InPath;
   double rho[3];
};

struct BodyPathTableType { /* tells if inner body is in path of outer body*/
   /*~ Internal Variables ~*/
   long InPath;
   double Coi[3][3];
};

struct DynType {
   /*~ Internal Variables ~*/
   long Nu;   /* 6 + Sum(Joint DOFs) */
   long Nx;   /* 7 + Sum(Joint DOFs), (spherical gives 4) */
   long **ConnectTable;  /* Nb x Ng */
   struct JointPathTableType **JointPathTable;  /* Nb x Ng */
   struct BodyPathTableType **BodyPathTable;  /* Nb x Nb */
   double **PAngVel; /* 3*Nb x Nu */
   double **IPAngVel; /* 3*Nb x Nu */
   double **PVel;   /* 3*Nb x Nu */
   double **mPVel;   /* 3*Nb x Nu */
   double *BodyTrq;  /* 3*Nb x 1 */
   double *BodyFrc;  /* 3*Nb x 1 */
   double **COEF;  /* (Nu+Nf) x (Nu+Nf) */
   double *RHS;    /* (Nu+Nf) x 1 */
   long SomeJointsLocked; /* 1 if any DOFs are locked */
   long Ns;  /* Number of active states (joint + flex), <= (Nu+Nf) */
   double *ActiveState; /* u and uf concatenated, (Nu+Nf) x 1 */
   long *ActiveStateIdx;  /* Keeps track of active states, (Nu+Nf) x 1 */
   double *u,*uu,*du,*udot;  /* Nu  (Dynamic States) */
   double *x,*xx,*dx,*xdot;  /* Nx  (Kinematic States) */
   double *h,*hh,*dh,*hdot;  /* Nw  (Wheel Momentum States) */
   double *a,*aa,*da,*adot;  /* Nw  (Wheel spin angle states) */
   /* For Flex */
   long Nf;  /* Total Number of Flex Modes, Sum(B.Nf) */
   double **PAngVelf; /* 3*Nb x Nf */
   double **IPAngVelf; /* 3*Nb x Nf */
   double **PVelf; /* 3*Nb x Nf */
   double **mPVelf; /* 3*Nb x Nf */
   double **Mf; /* Nf x Nf */
   double **PCPVelf; /* Nf x Nf */
   double **HplusQetaPAngVelf; /* Nf x Nf */
   double *uf,*uuf,*duf,*ufdot; /* Nf (Dynamic Flex States) */
   double *xf,*xxf,*dxf,*xfdot; /* Nf (Kinematic Flex States) */
   double *FlexAcc;  /* Nf x 1 */
   double *FlexFrc;  /* Nf x 1 */
   double *FlexInertiaFrc; /* Nf x 1 */
   /* For Constraints */
   long Nc;  /* 6*Nb - Nu */
   double **PAngVelc; /* 3*Nb x Nc */
   double **PVelc; /* 3*Nb x Nc */
   double *TotalTrq; /* 3*Nb x 1 */
   double *TotalFrc; /* 3*Nb x 1 */
   double *GenConstraintFrc;  /* Nc x 1 */
};

struct EnvTrqType {
   /*~ Internal Variables ~*/
   long First;
   FILE *envfile;
   double Hs[3];
};

struct IpcType {
   /*~ Internal Variables ~*/
   long Init;
   long Enabled;
   long Mode; /* OFF, TX, RX, TXRX, WRITEFILE, READFILE */
   long SocketRole; /* SERVER, CLIENT, GMSEC_CLIENT */
   long AcsID;  /* AC.ID for ACS mode */
   char HostName[40];
   long Port;
   long AllowBlocking;
   long EchoEnabled;
   SOCKET Socket;
   FILE *File;
   long Nprefix;
   char **Prefix;
};

struct SCType {
   /*~ Outputs ~*/
   double qn[4]; /* Attitude of Body 0 in N [~=~] */
   double wn[3]; /* Angular rates of Body 0 in N [[rad/sec]] [~=~] */
   double PosR[3]; /* Position of cm wrt Reference Orbit [[m]], expressed in N [~=~] */
   double VelR[3]; /* Velocity of cm wrt R [[m/s]], expressed in N [~=~] */
   double svb[3]; /* Sun-pointing unit vector, expressed in SC.B[0] [~=~] */
   double bvb[3]; /* Magfield [[Tesla]], expressed in SC.B[0] [~=~] */
   double Hvb[3]; /* Total SC angular momentum [[Nms]], expressed in SC.B[0] [~=~] */

   /*~ Internal Variables ~*/
   long ID;     /* SC[x].ID = x */
   long Exists;
   char Label[40];
   long DynMethod;  /* GAUSS_ELIM, ORDER_N */
   long OrbDOF;  /* FIXED, EULER_HILL, ENCKE, COWELL */
   long RefOrb;
   long FswTag;  /* Tag for FSW function, eg. INSTANT_FSW */
   double FswSampleTime;
   long FswMaxCounter;
   long FswSampleCounter;
   long InitAC;

   long Nb;   /* Number of bodies */
   long Ng;   /* Number of joints, = Nb-1 */
   
   long Nw;   /* Number of wheels */
   long Nmtb; /* Number of MTB's */
   long Nthr; /* Number of thrusters */
   
   long Ngyro; /* Number of Gyro axes */
   long Nmag; /* Number of magnetometer axes */
   long Ncss; /* Number of coarse sun sensors */
   long Nfss; /* Number of Fine Sun Sensors */
   long Nst; /* Number of star trackers */
   long Ngps; /* Number of GPS receivers */
   long Nacc; /* Number of accelerometer axes */
   long Nfgs; /* Number of Fine Guidance Sensors */
   long Nsh; /* Number of shakers */
   
   double mass;
   double cm[3]; /* wrt B0 origin, expressed in B0 frame */
   double I[3][3]; /* Inertia matrix, wrt SC.cm, expressed in B0 frame */
   double PosEH[3];  /* Position of cm wrt R, m, in Euler-Hill coords */
   double VelEH[3];  /* Velocity of cm wrt R, m, in Euler-Hill coords */
   double PosN[3];   /* Position of cm wrt origin of N, m, expressed in N */
   double VelN[3];   /* Velocity of cm wrt origin of N, m/sec, expressed in N */
   double CLN[3][3]; /* Note that SC.CLN != Orb[RefOrb].CLN if SC.PosR != 0.0 */
   double CEN[3][3]; /* E = Equatorial frame: e1 = North, e2 = East, e3 = Nadir */
   double wln[3]; /* Expressed in N */
   double PosH[3];  /* Position of cm wrt H frame, expressed in H */
   double VelH[3];  /* Velocity of cm wrt H frame, expressed in H */
   double FrcN[3]; /* Force, N, expressed in N */
   double AccN[3]; /* Acceleration due to external force, for accelerometer model */
   double svn[3]; /* Sun-pointing unit vector, expressed in N */
   double bvn[3]; /* Magfield, Tesla, expressed in N */
   double Hvn[3]; /* Total SC angular momentum, Nms, expressed in N */
   long Eclipse;
   double AtmoDensity;
   double DragCoef;
   char FileName[20];
   char SpriteFileName[40];
   unsigned int SpriteTexTag;
   /* The following are for OSCAR */
   double PosF[3]; /* Position of B0 origin wrt F, expressed in F */
   double VelF[3]; /* Velocity of B0 origin wrt F, expressed in F */
   double CF[3][3];  /* Attitude of B0 wrt F */
   /* Constraint forces and torques are computed if requested */
   long ConstraintsRequested;
   /* Mass and flex properties referred to REFPT_CM or REFPT_JOINT */
   long RefPt;
   /* Flexible Dynamics Active */
   long FlexActive;
   /* Include higher-order coupling terms in rigid-flex dynamics */
   long IncludeSecondOrderFlexTerms;
   char ShakerFileName[40];
   long WhlDragActive;
   long WhlJitterActive;
   /* Workspace for KaneNBody */
   struct DynType Dyn;
   /* Workspace for Actuator Sizing */
   struct EnvTrqType EnvTrq;
   /* Bounding Box used for shadowmap */
   struct BoundingBoxType BBox;
   /* See ReadStatesFromSocket */
   long RequestStateRefresh;
   
   /* For stability analysis */
   long GainAndDelayActive;
   double LoopGain;
   double LoopDelay;
   
   /*~ Structures ~*/
   struct AcType AC;
   struct BodyType *B;           /* [*Nb*] */
   struct JointType *G;          /* [*Ng*] */
   struct JointType GN;          /* Joint between N and B[0] */
   struct IdealActType IdealAct[3];
   struct WhlType *Whl;          /* [*Nw*] */
   struct MTBType *MTB;          /* [*Nmtb*] */
   struct ThrType *Thr;          /* [*Nthr*] */
   struct GyroType *Gyro;        /* [*Ngyro*] */
   struct MagnetometerType *MAG; /* [*Nmag*] */
   struct CssType *CSS;          /* [*Ncss*] */
   struct FssType *FSS;          /* [*Nfss*] */
   struct StarTrackerType *ST;   /* [*Nst*] */
   struct GpsType *GPS;          /* [*Ngps*] */
   struct AccelType *Accel;      /* [*Nacc*] */
   struct FgsType *Fgs;          /* [*Nfgs*] */
   struct ShakerType *Shaker;    /* [*Nsh*] */
   
   #ifdef _AC_STANDALONE_
   struct AcIpcType AcIpc;
   #endif
};

struct TargetType {
   /*~ Internal Variables ~*/
   long Type;
   long World;
   long RefOrb;
   long SC;
   long Body;
   double PosR[3];
   double PosN[3];
   double PosH[3];
   double CN[3][3];
};

struct POVType {
   /*~ Internal Variables ~*/
   long Mode; /* Track Host, Track Target, or Fixed in Host */
   struct TargetType Host;
   struct TargetType Target;
   long View;
   long Frame;  /* Which frame is POV frame oriented in? (N=0,L=1,F=2,B=3) */
   long BoreAxis;  /* POV boresight axis (out of screen): POS_X, POS_Y, ..., NEG_Z */
   long UpAxis;    /* POV axis pointing to top of window: POS_X, ... NEG_Z */
   double Width; /* Width of POV Field of View */
   double Height; /* Height of POV Field of View */
   double Near, Far;  /* Near and Far limits of POV FOV */
   double CosFov, SinFov;
   double Angle; /* Angle subtended in vertical, deg */
   double AR;    /* Aspect ratio of POV FOV */
   double PosLeftEye[3]; /* in POV frame, expressed in POV */
   double PosRightEye[3]; /* in POV frame, expressed in POV */
   double w[3];  /* Angular velocity */
   double q[4];  /* Quaternion */
   double PosB[3];  /* Position wrt Host, expressed in Host B[0] Frame */
   double Range;
   double GridSpacing; /* For ProxOps Grid */
   double wmax[3];
   double C[3][3];
   double CN[3][3];
   double CH[3][3];
   double CL[3][3];
   double CF[3][3];
   double CB[3][3];
   double PosR[3]; /* Position vector in R, expressed in N */
   double PosN[3]; /* Position vector in N, expressed in N */
   double PosH[3]; /* Position vector in H, expressed in H */
   float ViewMatrix[16];
   /* For PanZoomPOV */
   double TimeToGo;
   long CmdSeq;
   double CmdAngle[3];
   double CmdRange;
   double CmdPermute[3][3];
};

struct RegionType {
   /*~ Internal Variables ~*/
   long Exists;
   long World;
   double Lng,Lat,Alt; /* Origin location */
   double PosW[3];
   double CW[3][3]; /* Region frame is East-North-Up */
   double PosN[3];
   double VelN[3];
   double CN[3][3];
   double wn[3]; /* Expressed in R frame */
   double ElastCoef,DampCoef,FricCoef;
   char Name[20];
   char MeshFileName[40];
   long MeshTag;
   float ModelMatrix[16]; /* For OpenGL */
};

struct AtmoType {
   /*~ Internal Variables ~*/
   long Exists;
   float GasColor[3];
   float DustColor[3];
   float RayScat[3];
   float MieScat;
   float RayScaleHt;
   float MieScaleHt;
   float MieG;
   double MaxHt;
   double rad;
};

struct WorldType {
   /*~ Outputs ~*/
   double PosH[3];         /* Position in H frame [~=~] */
   
   /*~ Internal Variables ~*/
   
   /* Relationships */
   long Exists;
   long Type; /* STAR, PLANET, MOON, ASTEROID, COMET */
   long Parent;
   long Nsat;
   long *Sat; /* [*Nsat*] */

   /* Physical Properties */
   double mu; /* Gravitation constant  */
   double J2; /* Gravitation oblateness parameter */
   double rad; /* Radius */
   double w; /* Spin Rate */
   double PriMerAngJ2000; /* Prime Meridian Angle at J2000 epoch, rad */
   double RadOfInfluence; /* Radius of Sphere of Influence */
   double DipoleMoment;  /* Magnetic Field Dipole Moment, Wb-m */
   double DipoleAxis[3]; /* Magnetic Field Dipole Axis */
   double DipoleOffset[3]; /* Dipole Offset, m */
   double RingInner, RingOuter;
   double Density; /* For minor bodies, polyhedron gravity */

   /* Graphical Properties */
   long HasRing;
   char Name[20];
   char MapFileName[40];
   char MeshFileName[40];
   char ColTexFileName[40];
   char BumpTexFileName[40];
   float Color[4];
   unsigned char Glyph[14];
   unsigned int TexTag;
   unsigned int MapTexTag;
   unsigned int ColTexTag;
   unsigned int BumpTexTag;
   unsigned int ColCubeTag;
   unsigned int BumpCubeTag;
   unsigned int CloudGlossCubeTag;
   long MeshTag;
   unsigned int RingTexTag;
   double NearExtent,FarExtent;

   double CNH[3][3]; /* DCM from heliocentric ecliptic frame to world-centric equatorial inertial frame */
   double qnh[4]; 
   double CNJ[3][3]; /* DCM from J2000 frame to world-centric equatorial inertial frame */
   double qnj[4];

   double VelH[3];         /* Velocity in H frame */
   double PriMerAng; /* Angle from N1 to prime meridian */
   double CWN[3][3]; /* DCM from world-centric inertial frame to world-centric rotating frame */
   double qwn[4]; 
   long Visibility; /* Too small to see, point-sized, or shows disk */
   float ModelMatrix[16];

   /*~ Structures ~*/
   struct OrbitType eph; /* Ephemeris */
   struct AtmoType Atmo;
};

struct SpotType {
   /*~ Internal Variables ~*/
   long xmin,ymin;
   long xmax,ymax;
   long Visible;
   long Selected;
};

struct WidgetType {
   /*~ Internal Variables ~*/
   long xmin,ymin;
   long xmax,ymax;
   float BorderColor[4];
   float TextColor[4];
   float SelectedColor[4];
   float UnselectedColor[4];
   long Nspot;
   struct SpotType *Spot;
};

struct FovType {
   /*~ Internal Variables ~*/
   long Type; /* WIREFRAME, SOLID, or VECTOR */
   long NearExists;
   long FarExists;
   long RefOrb;
   long SC;
   long Body;
   char Label[40];
   long Nv;
   long BoreAxis; /* X_AXIS, Y_AXIS, Z_AXIS */
   long H_Axis; /* (BoreAxis+1)%3 */
   long V_Axis; /* (BoreAxis+2)%3 */
   double Width; /* X angular dimension, rad */
   double Height; /* Y angular dimension, rad */
   double Length;
   double pb[3];
   double CB[3][3];
   float Color[4];
};

struct TdrsType {
   /*~ Internal Variables ~*/
   long Exists;
   double JD;
   double rw[3]; /* Position vector in ECEF frame */
   double PosN[3];  /* Position vector in N frame */
   double VelN[3];  /* Velocity vector in N frame (never used?) */
   double lat;
   double lng;
   char Designation[40];
};

struct GroundStationType {
   /*~ Internal Variables ~*/
   long Exists;
   long World;
   long Show;
   double lng, lat;
   double PosW[3];  /* Position vector in World frame */
   char Label[40];
};

/* Framebuffer Objects for Spacecraft Shadows */
struct ShadowFBOType {
   /*~ Internal Variables ~*/
   unsigned int FrameTag;
   unsigned int Height, Width;
   unsigned int RenderTag;
   unsigned int TexTag;
   float *Tex;
};

struct AlbedoFBOType {
   /*~ Internal Variables ~*/
   unsigned int FrameTag;
   unsigned int Height, Width;
   unsigned int RenderTag;
   unsigned int TexTag;
   float *Tex;
};

/* Orrery POV is different from POV */
struct OrreryPOVType {
   /*~ Internal Variables ~*/
   long Regime;  /* CENTRAL or THREE_BODY */
   long CenterType;
   long World;
   long LagSys;
   long MinorBody;
   long LP;
   double PosN[3]; /* Position wrt World, expressed in World N */
   double Radius;
   long Zoom;
   double Scale[30];
   char ScaleLabel[30][8];
   double Angle;
   double CNH[3][3];
   double CN[3][3];
   double CH[3][3];
   double CL[3][3];
};

struct ConstellationType {
   /*~ Internal Variables ~*/
   char Tag[4]; 
   long Class;  /* MAJOR, ZODIAC, or MINOR */
   long Nstars;
   long Nlines;
   double **StarVec;
   /* For each line */
   long *Star1;
   long *Star2;
};


/*
** #ifdef __cplusplus
** }
** #endif
*/

#endif /* __42TYPES_H__ */
