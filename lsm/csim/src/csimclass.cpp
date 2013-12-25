
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "csimclass.h"
#include "csimerror.h"


csimClassInfo *csimClass::classInfo=0;

csimClassInfoDB TheCsimClassDB;

csimClassInfo::csimClassInfo(void)
{
  nRegFields = 0;
  lRegFields = 1;
  regFields = (csimFieldInfo **)malloc(lRegFields*sizeof(csimFieldInfo *));
  arraySize = 0;
}

csimClassInfo::~csimClassInfo()
{
  for(int i=0;i<nRegFields;i++)
    delete regFields[i];

  if  (regFields) free(regFields); regFields=0;
}

int csimClassInfo::add(csimFieldInfo *fi)
{
  if ( ++nRegFields > lRegFields ) {
    lRegFields += 2;
    regFields = (csimFieldInfo **)realloc(regFields,lRegFields*sizeof(csimFieldInfo *));
  }
  regFields[nRegFields-1] = fi;
  if ( fi->access == READWRITE ) {
    arraySize += (fi->m);
  }
  return 0;
}

   const char *name;
   const char *description;
   const char *units;
   int access2;
   int m;
   float lb,ub;

#define SETFIELDINFO(type) csimFieldInfo *fi = new csimFieldInfo; \
                       fi->name        = name; \
                       fi->description = description; \
                       fi->m           = sz; \
                       fi->access      = access; \
                       fi->lb          = (float)lb; \
                       fi->ub          = (float)ub; \
                       fi->units       = units; \
                       fi->offset=((char *)(field))-(base); \
                       fi->fieldType=type; \
                       add(fi); \
                       return 0;

int csimClassInfo::registerField(char *base, const char *name, pDoubleField field, int access, int sz, double lb, double ub, const char *units, const char *description)
{
  SETFIELDINFO(DOUBLEFIELD);
}


int csimClassInfo::registerField(char *base, const char *name, pDoubleArray field, int access, int sz, double lb, double ub, const char *units, const char *description)
{
  SETFIELDINFO(DOUBLEARRAY);
}

int csimClassInfo::registerField(char *base, const char *name, pFloatField field, int access, int sz, double lb, double ub, const char *units, const char *description)
{
  SETFIELDINFO(FLOATFIELD)
}

int csimClassInfo::registerField(char *base, const char *name, pFloatArray field, int access,  int sz, double lb, double ub, const char *units, const char *description)
{
  SETFIELDINFO(FLOATARRAY);
}

int csimClassInfo::registerField(char *base, const char *name, pIntField field, int access, int sz, double lb, double ub, const char *units, const char *description)
{
  SETFIELDINFO(INTFIELD);
}

int csimClassInfo::registerField(char *base, const char *name, pIntArray field, int access, int sz, double lb, double ub, const char *units, const char *description)
{
  SETFIELDINFO(INTARRAY);
}

int csimClassInfo::getFieldId(char *fname)
{
  for(int i=0;i<nRegFields;i++)
    if ( strcmp(regFields[i]->name,fname) == 0 ) { return i; }
  
  TheCsimError.add("csimClassInfo::getFieldId: class %s has no field named %s\n",this->name,fname);
  return -1;
}

#if !defined(max)
#define max(A, B) ((A) > (B) ? (A) : (B))
#endif

void csimClassInfo::listFields(void)
{
  int i,l,mnl = 0,mll = 0, mul=0, mUl=0;
  char ustr[256];
  for(i=0;i<nRegFields;i++) {
    csimFieldInfo *f = regFields[i];

    l=strlen(f->name); mnl=max(mnl,l);

    sprintf(ustr,"%g",f->lb); l=strlen(ustr); mll=max(mll,l);
    sprintf(ustr,"%g",f->ub); l=strlen(ustr); mul=max(mul,l);

    l=strlen(f->units); mUl=max(mUl,l);
  }
  for(i=0;i<nRegFields;i++) {
    csimFieldInfo *f = regFields[i];
    if ( f->access == READWRITE ) {
      if ( f->units[0] == '\0' )
        csimPrintf("  %*s = %s range: %g to %g\n",mnl,f->name,f->description,f->lb,f->ub);
      else
        csimPrintf("  %*s = %s range: %g to %g; units: %s\n",mnl,f->name,f->description,f->lb,f->ub,f->units);
    }
  }
  for(i=0;i<nRegFields;i++) {
    csimFieldInfo *f = regFields[i];
    if ( f->access == READONLY ) {
      if ( f->units[0] != '\0' )
        csimPrintf("  %*s : %s units: %s\n",mnl,f->name,f->description,f->units);
      else
        csimPrintf("  %*s : %s\n",mnl,f->name,f->description);
    }
  }
}

csimClassInfoDB::csimClassInfoDB(void)
{
  nRegClasses = 0;
  lRegClasses = 1;
  regClasses = (csimClassInfo **)malloc(lRegClasses*sizeof(csimClassInfo *));

  registerClasses();
}

csimClassInfoDB::~csimClassInfoDB(void)
{
  //  csimPrintf("freeing TheCsimClassInfoDB\n");
  for(int i=0;i<nRegClasses;i++)
    delete regClasses[i];

  if (regClasses) free(regClasses); regClasses=0;
}

int csimClassInfoDB::add(csimClassInfo *ci) 
{
  if ( ++nRegClasses > lRegClasses ) {
    lRegClasses += 2;
    regClasses = (csimClassInfo **)realloc(regClasses,lRegClasses*sizeof(csimClassInfo *));
  }
  regClasses[nRegClasses-1] = ci;
  return 0;
}

csimClassInfo *csimClassInfoDB::registerCsimClass(const char *name, const char *description)
{
  csimClassInfo *info = new csimClassInfo;

  info->name = name;
  info->description = description;
  info->classId = nRegClasses;

  //  c->setClassInfo(info);

  add(info);
  // csimPrintf("registring %s:  add done\n",name);
  //print();
  return info;
}

#include "classlist.i"
#include "registerclasses.i"

void csimClassInfoDB::listClasses(bool listFields)
{
  int wmax=0,l,c;
  csimPrintf("\n");
  for(c=0;c<nRegClasses;c++) {
    if ( (l=strlen(regClasses[c]->name)) > wmax ) wmax = l;
  }
  for(c=0;c<nRegClasses;c++) {
    if ( listFields ) { 
      csimPrintf("%s: %s\n",regClasses[c]->name,regClasses[c]->description);
      regClasses[c]->listFields();
      csimPrintf("\n");
    } else {
      csimPrintf("%*s : %s\n",wmax,regClasses[c]->name,regClasses[c]->description);
    }
  }
  csimPrintf("\n");
}

// csimClassInfo *csimClass::classInfo;

csimClass::csimClass(void)
{
  Id = -1; dirty=1;
}

int csimClass::setFieldByName(char *o, char *name, double *v)
{
  return setFieldById(o,getFieldId(name),v);
}

int csimClass::setFieldById(char *o, int id, double *v)
{
  if ( id < 0 || id >= getClassInfo()->nRegFields )
    { TheCsimError.add("csimClass::setFieldById: invalid field Id (%i) for class %s\n",id,className()); return -1; }

  csimFieldInfo *fi=getClassInfo()->regFields[id];

  if ( fi->access != READWRITE ) 
    { TheCsimError.add("csimClass::setFieldById: field %s is READONLY!\n",getFieldName(id)); return -1; }

  int i;

  //  printf("%i %i %i\n",fi->ff,fi->df,fi->intf);

  switch ( fi->fieldType ) {
  case FLOATFIELD:
    *((float *)(o+(fi->offset))) = (float)(*v);
    break;
    
  case DOUBLEFIELD:
    *((double *)(o+(fi->offset))) = (double)(*v);
    break;
    
  case INTFIELD:
    *((int *)(o+(fi->offset))) = (int)(*v);
    break;
    
  case FLOATARRAY:
    for(i=0; i<fi->m; i++) {
      (*(*((float **)(o+(fi->offset)))+i)) = (float)v[i];
    }
    break;
    
  case DOUBLEARRAY:
    memcpy(*((double **)(o+(fi->offset))),v,fi->m*sizeof(double));
    break;
    
  case INTARRAY:
    for(i=0; i<fi->m; i++) {
      (*(*((int **)(o+(fi->offset)))+i)) = (int)v[i];
    }
    break;
    
  default: { 
      TheCsimError.add("csimClass::setFieldById: internal error this should not happen!!\n"); 
      return -1; 
    }
  }
  
  dirty = 1;

  return 0;
}

int csimClass::getFieldByName(char *o, char *name, double *v)
{
  return getFieldById(o,getFieldId(name),v);
}

int csimClass::getFieldById(char *o, int id, double *v)
{
  if ( id < 0 || id >= getClassInfo()->nRegFields )
    { TheCsimError.add("csimClass::getFieldById: invalid field Id (%i) for class %s\n",id,className()); return -1; }
 
  csimFieldInfo *fi=getClassInfo()->regFields[id];

  int i;

  switch ( fi->fieldType ) {
  case FLOATFIELD:
    *v = *((float *)(o+(fi->offset)));
    break;
    
  case DOUBLEFIELD:
    *v = *((double *)(o+(fi->offset)));
    break;
    
  case INTFIELD:
    *v = *((int *)(o+(fi->offset)));
    break;
    
  case FLOATARRAY:
    for(i=0; i<fi->m; i++)
      v[i] = (*(*((float **)(o+(fi->offset)))+i));
    break;
    
  case DOUBLEARRAY:
    memcpy(v,*((double **)(o+(fi->offset))),fi->m*sizeof(double));
    break;
    
  case INTARRAY:
    for(i=0; i<fi->m; i++)
      v[i] = (*(*((int **)(o+(fi->offset)))+i)); 
    break;
    
  default: { 
      TheCsimError.add("csimClass::getFieldById: internal error this should not happen!!\n"); 
      return -1; 
    }
    
  }
  return 0;
}

int csimClass::getFieldSizeByName(char *o, char *name)
{
  return getFieldSizeById(o,getFieldId(name));
}

int csimClass::getFieldSizeById(char *, int id)
{
  if ( id < 0 || id >= getClassInfo()->nRegFields )
    { TheCsimError.add("csimClass::getFieldSizeById: invalid field Id (%i) for class %s\n",id,className()); return -1; }
 
  return getClassInfo()->regFields[id]->m;
}

char *csimClass::getFieldPointerById(char *o, int id)
{
  if ( id < 0 || id >= getClassInfo()->nRegFields )
    { TheCsimError.add("csimClass::getFieldPointerById: invalid field Id (%i) for class %s\n",id,className()); return 0; }
 
  return (o+(getClassInfo()->regFields[id]->offset));
}

int csimClass::getFieldTypeById(int id)
{
  if ( id < 0 || id >= getClassInfo()->nRegFields )
    { TheCsimError.add("csimClass::getFieldTypeById: invalid field Id (%i) for class %s\n",id,className()); return 0; }
 
  return getClassInfo()->regFields[id]->fieldType;
}

void csimClass::printFields(char *o)
{
  csimClassInfo *info=getClassInfo();
  int i,m,l,wname=0;
  csimPrintf("%i : %s",Id,info->name);
  if ( info->nRegFields > 0 ) {
    // determine maximum length of field names
    for(i=0;i<info->nRegFields;i++)
      if ( (l=strlen(info->regFields[i]->name)) > wname ) wname = l;
    
    // print each field
    for(i=0;i<info->nRegFields;i++) { 
      if ( (m=getFieldSizeById(o,i)) == 1 ) {
	// field is a single value
	double v;
	getFieldById(o,i,&v);
        char c = info->regFields[i]->access == READWRITE ? '=' : ':' ;
	if ( info->regFields[i]->units[0] != 0 )
	  csimPrintf("\n %*s %c %g (%s)",wname,info->regFields[i]->name,c,v,info->regFields[i]->units);
	else
	  csimPrintf("\n %*s %c %g",wname,info->regFields[i]->name,c,v);
      } else if ( m <  6 ) {
	// field is a short vector
	// 1) alloc some memory to write values
	double *v=(double *)malloc(m*sizeof(double));
	// 2) get values
	getFieldById(o,i,v);
	// 3) print them
	csimPrintf("\n  %*s = [ ",wname,info->regFields[i]->name);
	for(int j=0;j<m;j++) csimPrintf("%g ",v[j]);
	csimPrintf("] (%s)",info->regFields[i]->units);
	// 4) free memory
	if (v) free(v); v = 0;
      } else
	// field to long to print out
	csimPrintf("\n  %*s = [ 1 x %i array ] %s",wname,info->regFields[i]->name,m,info->regFields[i]->units);
    }
    csimPrintf("\n");
  } else {
    csimPrintf(" no fields.\n");
  }
  csimPrintf("\n");
}


double *csimClass::getFieldArray(char *o)
{
  csimClassInfo *info=getClassInfo();
  double *array = (double *)malloc(info->getFieldArraySize()*sizeof(double));
  double *p = array;
  for(int i=0;i<info->nRegFields;i++) {
    if ( info->regFields[i]->access == READWRITE) { 
      getFieldById(o,i, p);
      p += (info->regFields[i]->m);
    }
  }
  return array;
}

int csimClass::setFieldArray(char *o, double *p)
{
  csimClassInfo *info=getClassInfo();
  for(int i=0;i<info->nRegFields;i++) {
    if ( info->regFields[i]->access == READWRITE) { 
      setFieldById(o,i, p);
      p += (info->regFields[i]->m);
    }
  }
  dirty = 1;
  return 0;
}










