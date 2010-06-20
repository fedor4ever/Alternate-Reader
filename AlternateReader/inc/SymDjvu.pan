
#ifndef SYMDJVU_PAN_H
#define SYMDJVU_PAN_H

/** SymDjvu application panic codes */
enum TSymDjvuPanics
	{
	ESymDjvuUi = 1
	// add further panics here
	};

inline void Panic(TSymDjvuPanics aReason)
	{
	_LIT(applicationName,"SymDjvu");
	User::Panic(applicationName, aReason);
	}

#endif // SYMDJVU_PAN_H
