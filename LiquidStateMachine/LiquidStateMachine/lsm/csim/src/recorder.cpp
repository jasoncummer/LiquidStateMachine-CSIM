/*! \file recorder.cpp
**  \brief Implementation of Recorder
*/

#include "csimclass.h"
#include "recorder.h"
#include "csimerror.h"
#include "spikingneuron.h"

csimRecorder::csimRecorder(void)
{
  nRecFields = 0;
  lRecFields = 0;
  recFields  = 0;

  v = values  = 0;
  nValues     = 0;
  lValues     = 0;

  Tprealloc = 0.5;  
  dt        = 5e-3;
  nRecordSteps = 0;
  nAnalogFields = 0;
  enabled = 1;
}

csimRecorder::~csimRecorder(void)
{
  for(int i=0;i<nRecFields;i++)
    delete recFields[i];

  if (recFields) free(recFields); recFields = 0;
  
  if (values) free(values); values = 0;
}


int csimRecorder::addFieldToRecord(csimClass *o, char *fieldname)
{
  if ( !o ) {
    TheCsimError.add("csimRecorder::addFieldToRecord: NULL pointer specified!\n");
    return -1;
  }
  if ( nValues > 0 ) {
    TheCsimError.add("csimRecorder::addFieldToRecord: not allowed to connect a new field during simulation!\n"
		     "Call csim('reset') first.");
    return -1;
  }

  RecField *f=new RecField;

  f->p = 0; f->type = -1;

  if ( 0 == strcmp(fieldname,"spikes") ) {
    SpikingNeuron *n=dynamic_cast<SpikingNeuron *>(o);
    if ( n ) {
      f->i = -1;
    } else {
      TheCsimError.add("csimRecorder::addFieldToRecord: class %s has no spikes to record.\n",o->className(),fieldname);
      return -1;
    }
  } else {
    if ( (f->i=o->getFieldId(fieldname)) < 0 ) {
      TheCsimError.add("csimRecorder::addFieldToRecord: class %s has no '%s' field.\n",o->className(),fieldname);
      return -1;
    } else {
      nAnalogFields++;
      f->p    = o->getFieldPointerById((char *)o,f->i);
      f->type = o->getFieldTypeById(f->i);
      if ( o->getFieldSizeById((char *)o,f->i) > 1 ) {
	TheCsimError.add("csimRecorder::addFieldToRecord: Recording of fields with size > 1 not supported yet!"); 
	return -1;
      }
    }
  }

  f->o=o;

  add(f);
  
  return 0;
}

int  csimRecorder::add(RecField *f)
{
  if ( ++nRecFields > lRecFields ) {
    lRecFields += 10;
    recFields = (RecField **)realloc(recFields,lRecFields*sizeof(RecField  *));
  }
  recFields[nRecFields-1] = f;
  return 0;
}

int csimRecorder::updateInternal(void)
{
  mod = (int)(dt/DT+0.5);  
  if ( mod < 1 ) {
    TheCsimError.add("csimRecorder::updateInternal: local dt (%g) smaller that global dt (%g)",dt,DT); return -1;
  }
  return 0;
}

void csimRecorder::reset(void)
{
  // start recording from scratch
  nStepsToRec  = mod;
  v            = values;
  nValues      = 0;
  nRecordSteps = 0;
}

int csimRecorder::advance(void)
{
  if ( enabled && --nStepsToRec == 0 ) {
    
    if ( ( nValues += nAnalogFields) > lValues ) {
      // if we run out of memory we realloc some more memory
      lValues += ((int)(Tprealloc/dt)*nRecFields);
      values = (double *)realloc(values,lValues*sizeof(double));
      // recalc the new writting position
      v = values+(nValues-nAnalogFields);
    }

    // get the values and store them at position v
    for(int i=0;i<nRecFields;i++) 
      switch ( (recFields[i]->type) ) {
      case FLOATFIELD:
        *(v++) = *((float *)(recFields[i]->p));
        break;
      case DOUBLEFIELD:
        *(v++) = *((double *)(recFields[i]->p));
        break;
      case INTFIELD:
        *(v++) = *((int *)(recFields[i]->p));
        break;
      }
    
    // wait another mod time steps
    nStepsToRec = mod;
    nRecordSteps++;
  }
  return 1;
}
