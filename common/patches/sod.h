#ifndef SOD_H_
#define SOD_H_

#include "../struct_strategy.h"

class EQStreamIdentifier;
class EQWebStreamIdentifier;

namespace SoD {

	//these are the only public member of this namespace.
	extern void Register(EQStreamIdentifier &into);
	extern void Reload();



	//you should not directly access anything below..
	//I just dont feel like making a seperate header for it.

	class Strategy : public StructStrategy {
	public:
		Strategy();

	protected:

		virtual std::string Describe() const;
		virtual const ClientVersion GetClientVersion() const;

		//magic macro to declare our opcode processors
		#include "ss_declare.h"
		#include "sod_ops.h"
	};

};

#endif /*SOD_H_*/
