
#include "mexrecorder.h"
#include "spikingneuron.h"

Recorder::Recorder(void)
{ 
  commonChannels=0;
} 


mxArray *Recorder::getMxStructArray(void)
{
  if ( commonChannels ) {
    return getMxCommonTraces();
  } else {
    return getMxSeparateTraces();
  }
}

mxArray *Recorder::getMxCommonTraces(void)
{
  const char *field_names[] = { "data", "info" };

  mxArray *rec_a = mxCreateStructMatrix(1,1,2,field_names);

  if ( nRecFields*nRecordSteps > 0 ) {
    mxArray *data_a = mxCreateDoubleMatrix(nRecFields,nRecordSteps,mxREAL);
    memcpy(mxGetPr(data_a),values,nValues*sizeof(double));
    mxSetFieldByNumber(rec_a,0,0,data_a);
  } else {
    mxSetFieldByNumber(rec_a,0,0,mxCreateDoubleMatrix(0,0,mxREAL));
  }
  
  if ( nRecFields == 0 ) {
    mxSetField(rec_a,0,"info",mxCreateDoubleMatrix(0,0,mxREAL));  
  } else {
    const char *info_fields[] = { "idx", "fieldName" };
    mxArray *inf_a = mxCreateStructMatrix(1,nRecFields,2,info_fields);
    
    for(int i=0;i<nRecFields;i++) {
      RecField *f=recFields[i];
      //      csimPrintf("Recorder::getMxCommonTraces: f->i=%i, id=%i %s %s \n",
      //	 f->i,(f->o)->getId(),(f->o)->className(),(f->o)->getFieldName(f->i));
      mxArray *name_a = mxCreateString((f->o)->getFieldName(f->i));
      mxArray *idx_a  = mxCreateNumericMatrix(1,1,mxUINT32_CLASS,mxREAL);
      *((uint32 *)mxGetData(idx_a)) = f->o->getId();
      mxSetFieldByNumber(inf_a,i,0,idx_a);
      mxSetFieldByNumber(inf_a,i,1,name_a);
    }

    mxSetFieldByNumber(rec_a,0,1,inf_a);  
  }

  return rec_a;
}

mxArray *Recorder::getMxSeparateTraces(void)
{
  const char *fields[] = { "idx", "fieldName", "data", "spiking", "dt" };
  mxArray *channel_a = mxCreateStructMatrix(1,nRecFields,5,fields);

  int idx_i = mxGetFieldNumber(channel_a,"idx");
  int name_i = mxGetFieldNumber(channel_a,"fieldName");
  int data_i = mxGetFieldNumber(channel_a,"data");
  int spk_i = mxGetFieldNumber(channel_a,"spiking");
  int dt_i = mxGetFieldNumber(channel_a,"dt");

  double *s0=values;
  for(int i=0;i<nRecFields;i++) {
    RecField *f=recFields[i];

    mxArray *idx_a  = mxCreateNumericMatrix(1,1,mxUINT32_CLASS,mxREAL);
    *((uint32 *)mxGetData(idx_a)) = f->o->getId();

    mxSetFieldByNumber(channel_a,i,idx_i,idx_a);

    if ( f->type > -1 ) {
      mxArray *name_a = mxCreateString(f->o->getFieldName(f->i));
      mxSetFieldByNumber(channel_a,i,name_i,name_a);
      
      mxArray *data_a = mxCreateDoubleMatrix(1,nRecordSteps,mxREAL);
      double *d=mxGetPr(data_a);
      double *s=s0;
      for(int j=0;j<nRecordSteps;j++) {
	*d++ = *s; s += nAnalogFields;
      }
      mxSetFieldByNumber(channel_a,i,data_i,data_a);
      
      mxSetFieldByNumber(channel_a,i,spk_i,mxCreateScalarDouble(0));
      
      mxSetFieldByNumber(channel_a,i,dt_i,mxCreateScalarDouble(dt));

      s0++;
    } else { 
      SpikingNeuron *n;
      if ( (n=dynamic_cast<SpikingNeuron *>(f->o)) ) {
	mxArray *data_a = mxCreateDoubleMatrix(1,n->nSpikes(),mxREAL);
        n->copySpikes(mxGetPr(data_a));
	mxSetFieldByNumber(channel_a,i,data_i,data_a);

	mxSetFieldByNumber(channel_a,i,name_i,mxCreateString("spikes"));
	mxSetFieldByNumber(channel_a,i,spk_i,mxCreateScalarDouble(1));
	mxSetFieldByNumber(channel_a,i,dt_i,mxCreateScalarDouble(-1));
      }
    }
  }

  const char *cf[] = { "channel" };
  mxArray *ret_a = mxCreateStructMatrix(1,1,1,cf);
  mxSetFieldByNumber(ret_a,0,0,channel_a);
  return ret_a;
}
