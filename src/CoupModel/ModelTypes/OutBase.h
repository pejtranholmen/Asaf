#pragma once
class ANN_INFO {
public:
	int ichart;
	int sequence;
};

#include "./SimB.h"
class OutBase: public SimB
{
	public:
	~OutBase(void);
	OutBase(enum datatype data, enum elements elem, enum fysprocess fproc, enum bioprocess bproc, enum userlevel ulev);
	bool AreAllFlagsSet();
	virtual size_t GetNumberOfSelectedFlags(bool multi=false) const {return 0;};
	size_t GetNumberOfFlags() const;

	virtual double	GetValue(size_t) {return MISSING;};
	virtual double	GetValue() {return MISSING;};
	void	ReSize(size_t);
	bool	Reallocate();			// set values to tmpval
	void	Reset();			// resets tmpvalues to values
}
;
