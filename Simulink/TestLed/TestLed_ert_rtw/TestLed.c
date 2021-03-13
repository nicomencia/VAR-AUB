/*
 * TestLed.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "TestLed".
 *
 * Model version              : 1.1
 * Simulink Coder version : 9.4 (R2020b) 29-Jul-2020
 * C source code generated on : Sat Mar 13 20:34:50 2021
 *
 * Target selection: ert.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "TestLed.h"
#include "TestLed_private.h"

/* Block states (default storage) */
DW_TestLed_T TestLed_DW;

/* Real-time model */
static RT_MODEL_TestLed_T TestLed_M_;
RT_MODEL_TestLed_T *const TestLed_M = &TestLed_M_;
real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/* Model step function */
void TestLed_step(void)
{
  real_T rtb_PulseGenerator;
  uint8_T tmp;

  /* DiscretePulseGenerator: '<Root>/Pulse Generator' */
  rtb_PulseGenerator = (TestLed_DW.clockTickCounter <
                        TestLed_P.PulseGenerator_Duty) &&
    (TestLed_DW.clockTickCounter >= 0) ? TestLed_P.PulseGenerator_Amp : 0.0;
  if (TestLed_DW.clockTickCounter >= TestLed_P.PulseGenerator_Period - 1.0) {
    TestLed_DW.clockTickCounter = 0;
  } else {
    TestLed_DW.clockTickCounter++;
  }

  /* End of DiscretePulseGenerator: '<Root>/Pulse Generator' */

  /* MATLABSystem: '<Root>/Digital Output' */
  rtb_PulseGenerator = rt_roundd_snf(rtb_PulseGenerator);
  if (rtb_PulseGenerator < 256.0) {
    if (rtb_PulseGenerator >= 0.0) {
      tmp = (uint8_T)rtb_PulseGenerator;
    } else {
      tmp = 0U;
    }
  } else {
    tmp = MAX_uint8_T;
  }

  writeDigitalPin(9, tmp);

  /* End of MATLABSystem: '<Root>/Digital Output' */
}

/* Model initialize function */
void TestLed_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(TestLed_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&TestLed_DW, 0,
                sizeof(DW_TestLed_T));

  /* Start for MATLABSystem: '<Root>/Digital Output' */
  TestLed_DW.obj.matlabCodegenIsDeleted = false;
  TestLed_DW.objisempty = true;
  TestLed_DW.obj.isInitialized = 1;
  digitalIOSetup(9, 1);
  TestLed_DW.obj.isSetupComplete = true;

  /* InitializeConditions for DiscretePulseGenerator: '<Root>/Pulse Generator' */
  TestLed_DW.clockTickCounter = 0;
}

/* Model terminate function */
void TestLed_terminate(void)
{
  /* Terminate for MATLABSystem: '<Root>/Digital Output' */
  if (!TestLed_DW.obj.matlabCodegenIsDeleted) {
    TestLed_DW.obj.matlabCodegenIsDeleted = true;
  }

  /* End of Terminate for MATLABSystem: '<Root>/Digital Output' */
}
