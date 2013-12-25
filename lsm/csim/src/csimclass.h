#ifndef __CSIMCLASS_H__
#define __CSIMCLASS_H__

/*! \file csimclass.h
**  \brief Class definitions of csimClass , csimFieldInfo , csimClassInfo , and csimClassInfoDB
*/


/*!

\page fields Setting and getting field values of objects

At the Matlab level CSIM allows you to set and get values of fields of
objects by means of the commands

\verbatim
  v=csim('get',o,fieldname)
  csim('set',o,fieldname,value)
\endverbatim


where \c o is the handle of the object returned by

\verbatim 
  o=csim('create',classname)
\endverbatim

and \c fieldname is a string identifying the field (i.e. the name of the field).


\section fields_implementation Implementaion

We implemented this mechanism using four classes:

  - csimClass : the base classes of all classes in CSIM which
  implements the basic set and get methodes for accessable fields

  - csimClassInfoDB : a container (of csimClassInfo objects) where
  information about all the (at the matlab level) available classes is
  stored

  - csimClassInfo : which stores information (accessible fields, description) about a certain class

  - csimFieldInfo : stores information about a certain field of a given class

\subsection fields_reg Registering classes and fields

To make the set and get methodes of a class derived from csimClass
work one has to register this class via
csimClassInfoDB::registerCsimClass() and then to register each member
variable which we want to be an accessable field via
csimClassInfo::registerField() .

Class member variable of the types 

  - \c double and \c double *
  - \c float and \c float *
  - \c int and \c int *

can be made accessible fields.

Each field has the following associated information (see csimFieldInfo )
- access : #READWRITE or #READONLY
- units
- lower and upper bound
- size : the number of elements if the field is of type \c float *, \c double *, or \c int *

\subsection reggen reggen

It woul be a tedious work to code all the
csimClassInfoDB::registerCsimClass() and
csimClassInfo::registerField() function calls by hand. So we decided
to generate it automatically from the source code. We use a tool named
\c reggen (a quick and dirty hack based on doxygen) to gather the
infomation from the source code and its doxygen style
documentation. \c reggen writes all relevant function calls to
register fields an classes to \c registerclass.i

\subsubsection def Default behaviour of \c reggen

- \c public class member variables are registered as
#READWRITE fields

- \c protected class member variables are
registered as #READONLY

- \c private class member variables are not
registered.

- no units are assumed (i.e. a dimensionless field)

- lower and upper bound are set to -Inf and +Inf respectively

- an warning is issued if no size information is given for arrays

- the brief \c doxygen comment is used as descriptio of the field

- the brief \c doxygen comment of a class is used as its description


\subsubsection spec Specifying information for use by reggen

If you want to register a class you just put the macro \c
#DO_REGISTERING somewhere in the class definition.

For each of the member variables where you want to change the default
behaviour of \c reggen you have to put a \c doxygen brief comment
where you specify the relevant information.  

\paragraph e1 Example 1
Make the float variable \c S a read-writable field with units Volt
and a lower and upper bound of -1 and +1 respectively:
\verbatim 
  //! A voltage scale factor [readwrite; units=Volt; range=(-1,1);]
  float S;
\endverbatim

\paragraph e2 Example 2
Make the double variable \c *A a read-only field with 20 elements, units Ohm,
and a lower and upper bound of -100 and +100 respectively:
\verbatim 
  //! A voltage scale factor [readonly; size=20; units=Ohm; range=(-100,100);]
  double *A;
\endverbatim

\subsubsection src Source code of reggen

- Currently reggen is maintained in \c lsm/develop/reggen .
- The relevant source file is \c lsm/develop/reggen/src/defgen.cpp . 
- The binaries are \c lsm/develop/reggen/bin/reggen for Linux and \c lsm/develop/reggen/bin/reggen.exe for windows.
- To compile reggen under Linux type

\verbatim
  cd lsm/develop/reggen; ./configure; make
\endverbatim

- To compile reggen under Windows XP with MS Visual C++ type

\verbatim
  cd lsm/develop/reggen; make.bat msvc
\endverbatim

*/

#include <string.h>
#include <stdio.h>
#include "globaldefinitions.h"


class csimClass;
class Network;
class Advancable;

typedef double * pDoubleField;
typedef double ** pDoubleArray;
typedef float * pFloatField;
typedef float ** pFloatArray;
typedef int  * pIntField;
typedef int ** pIntArray;

#define DOUBLEFIELD  0
#define DOUBLEARRAY  1 
#define FLOATFIELD   2
#define FLOATARRAY   3
#define INTFIELD     4
#define INTARRAY     5

//! Acces ID of read only fields
#define READONLY  0

//! Acces ID of read-writable fields
#define READWRITE 1

//! Holds information about a field of a class
/** See \link fields Setting and getting field values \endlink for more details. */
class csimFieldInfo {
public:
  csimFieldInfo(void) { offset = -1; fieldType = -1;};
  //! Name of the field
  const char *name;

  //! Short description of the field
  const char *description;
  
  //! Units of the field
  const char *units;

  //! Access: READONLY or READWRITE
  int access;

  //! Number of elements of the field (fileds can be arrays)
  int m;

  //! Lower bound of the field vlaue(s)
  float lb;

  //! Upper bound of the field vlaue(s)
  float ub;

  /*! @name fieldType and Offest of fields.  
   */
  //@{
  //! type
  int fieldType;
  //! offset
  int offset;
  //@}
};


//! Holds information about a class like name, description and field informations.
/** See also \link fields Setting and getting field values \endlink */
class csimClassInfo {
  
 public:
  csimClassInfo(void);
  ~csimClassInfo(void);
  
  //! Name of the class for which information is stored
  const char *name;

  //! Short description of the class
  const char *description;

  //! Id of the class (set by csimClassInfoDB)
  int  classId;

  /*! @name Methods for registering various variable types as fields */
  //@{
  //! for \c double
  int registerField(char *base, const char *name, pDoubleField field, int access, int sz, double lb, double ub, const char *units, const char *desc);
  //! for \c double *
  int registerField(char *base, const char *name, pDoubleArray array, int access, int sz, double lb, double ub, const char *units, const char *desc);
  //! for \c float
  int registerField(char *base, const char *name, pFloatField field, int access, int sz, double lb, double ub, const char *units, const char *desc);
  //! for \c float *
  int registerField(char *base, const char *name, pFloatArray array, int access, int sz, double lb, double ub, const char *units, const char *desc);
  //! for \c int
  int registerField(char *base, const char *name, pIntField field, int access, int sz, double lb, double ub, const char *units, const char *desc);
  //! for \c int *
  int registerField(char *base, const char *name, pIntArray array, int access, int sz, double lb, double ub, const char *units, const char *desc);
  //@}

  //! Return the Id of the field \c name
  int getFieldId(char *name);

  //! Return the number of fields of the corresponding class
  inline int nFields(void) { return nRegFields; }
  
  //! Return field name given the fiels ID
  inline const char *getFieldName(int id) { if ( id>=0 && id < nRegFields ) return regFields[id]->name; else return 0; }

  //! Return 1 if field is READWRITE
  inline bool isFieldRW(int id) { return (regFields[id]->access==READWRITE); }

  //! Print al list of fields to stdout
  void listFields(void);

  //! Return the number of doubles needed to store all READWRITE fields
  int getFieldArraySize(void) { return arraySize; };

 private:
  friend class csimClass;

  /*! Dirty implementation of a list of csimFieldInfo objects */
  //@{
  //! Add a field
  int add(csimFieldInfo *fi);
  //! Number of fields registerd
  int nRegFields;
  //! Number of slots currently allocated
  int lRegFields;
  //! The array
  csimFieldInfo **regFields;
  //@}

  //! Number of doubles needed to store all READWRITE fields
  int arraySize;
};

//! A database/dictionary of all classes  with accessable fields
/** 
 ** See \link fields Setting and getting field values \endlink for
 ** more details.
 **
 ** The main part of this class is the Function
 ** registerClasses(). Within this function all classes and fields
 ** have to be registerd. We use reggen to generate the implementation
 ** of registerClasses(). \c reggen writes this to \c
 ** registerclasses.i .
 **
 ** */
class csimClassInfoDB {
 public:
  csimClassInfoDB(void);
  ~csimClassInfoDB(void);

  //! Register a new class with \a name and description \a desc.
  /** \param name name of class to register 
   ** \param desc Short but meaningfull descpription 
   **
   ** This method does not store any informaotion
   ** about the field. Calls to csimClassInfo::registerField() have to
   ** follow. In our case these are automatically generated by \a
   ** reggen */
  csimClassInfo *registerCsimClass(const char *name, const char *desc);

  //! Within this function all registerCsimClass() calls must be made. 
  void registerClasses(void);

  //! Prints out the information about all registered classes. If F is set 1 also information about all fields is printed. 
  void listClasses(bool F);

 private:
  /* @name Quick implementation of a list of csimClassInfo objects */
  //@{
  //! Add a csimClassInfo object
  int add(csimClassInfo *info);
  //! Number of slots currently allocated
  int lRegClasses;
  //! Number of registered classes
  int nRegClasses;
  csimClassInfo **regClasses;
  //@}
};


extern csimClassInfoDB TheCsimClassDB;

//! Macro which should be put into the class definition of a class for which one wants accessable fields.
#define DO_REGISTERING    public: \
                            virtual csimClassInfo *getClassInfo(void) { return classInfo; }; \
                          protected: \
                            friend  class csimClassInfoDB; \
                            virtual void setClassInfo(csimClassInfo *ci){ classInfo = ci; }; \
                          private: \
                            static csimClassInfo *classInfo;

class Advancable ;

//! Base class of all classes in CSIM.
/** It provides the facilities to set/get class member variables
 ** specified by a string or Id. Class member variables (attributes)
 ** which can be get/set by means of the methods provided by csimClass
 ** will be called \e fiels. See \link fields Setting and getting
 ** field values \endlink
 **
 ** Classes derived from csimClass will have additional
 ** fields/parameters. We decided to make these public to avoid to
 ** define seperate \c set and \c get methods for each parameter
 ** (knowing to act against the philosophy of object oriented
 ** programming).
 **
 **  */
class csimClass {

 public:
  // The constructor (sets Id to -1)
  csimClass(void);
  
  // The destructor is needed bcs we have virtual function
  virtual ~csimClass(void){};

  //! Called only once directly after an object is created from Matlab via csim('create',className);
  virtual int init(Advancable *) { return 0; };

  //! Sets the field given by name to the values specified by v
  int setFieldByName(char *o, char *name, double *v);

  //! Sets the field given by its integer Id to the values specified by v
  int setFieldById(char *o, int id, double *v);

  //! Gets the value(s) of a field given by its name; v must point to a sufficiently large memory block
  int getFieldByName(char *o, char *name, double *v);

  //! Gets the value(s) of a field given by its integer Id; v must point to a sufficiently large memory block
  int getFieldById(char *o, int id, double *v);

  //! Get the size (number of doubles) of the field specified by name
  int getFieldSizeByName(char *o, char *name);

  //! Get the size (number of doubles) of the field specified by Id
  int getFieldSizeById(char *o, int id);
 
  //! Display the values of all fields of the object (with units)
  void printFields(char *o);

  //! Allocate and get a vector of doubles containing the values of all read/writeable fields.
  double *getFieldArray(char *o);

  //! Set all read/writeable fields given in an array of doubles containing the values of all the fields.
  int setFieldArray(char *o, double *p);

  //! Returns the size of the array returned by getFieldArray()
  inline int getFieldArraySize(void) { return getClassInfo()->getFieldArraySize(); }

  //! Get the Id of a field given its name
  inline int getFieldId(char *name) { return getClassInfo()->getFieldId(name); }

  //! Get the name of a field given its Id
  inline const char *getFieldName(int fId) { return getClassInfo()->getFieldName(fId); }

  //! Return the Id of the class the object belongs to
  inline int classId(void) { return getClassInfo()->classId; }

  //! Return the name of the class the object belongs to
  inline const char *className(void) { return getClassInfo()->name; }

  //! Get the unique Id of the object
  inline int getId(void) { return Id; }

  //! Wrapper function to get the correct value of classInfo
  virtual csimClassInfo *getClassInfo(void) { return classInfo; };

  //! Notify the object that some fields have been changed.
  /** This function will be called when we have to make sure that all
   ** the internal variables/states of an objact are consistent with
   ** the current values of the fields. */
  int fieldChangeNotify(bool force=0) { 
    if ( force || dirty ) { 
      return ((dirty=(updateInternal()!=0)) ? -1 : 0); 
    } else 
      return 0;
  };

  //! Update internal variables
  /** This function has to \b make sure that all the \b internal \b
   ** variables/states of an object are \b consistent with the
   ** current values of the fields. */
  virtual int updateInternal(void) { return 0; };

protected:
  friend  class csimClassInfoDB;

  //! A virtual wrapper function to set the correct value of classInfo
  virtual void setClassInfo(csimClassInfo *ci){ classInfo = ci; };

  friend class csimRecorder;

  /* **********************
     BEGIN MICHAEL PFEIFFER
     ********************** */
  friend class Readout;
  /* ********************
     END MICHAEL PFEIFFER
     ******************** */


  //! Get the pointer to a field by Id
  char *getFieldPointerById(char *o, int id);
  int getFieldTypeById(int id);

private:
  //! A pointer \b per \b class which points to the (field) information about this class
  static csimClassInfo *classInfo;
  
  friend class Network;
  //! The unique Id of the object.
  /** It is set if the object is inserted into the Network. */
  int Id;

  //! True if internal variables/states of an objact are inconsistent with the current values of the fields.
  /** Set to true if fields are changed via setFieldById,
   ** setFieldByName, or setFieldArray.  Set to 0 after a successful
   ** call to updateInternal. */
  bool dirty;

};


#endif
