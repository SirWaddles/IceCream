#ifndef _TRANSITION_HEADER_
#define _TRANSITION_HEADER_

namespace Util{
	class SteelTransition{
	public:
		SteelTransition();
		
		void Draw();
		void Think();

		void SetLength(unsigned int len);

		bool Ready();
		bool Complete();
		
	protected:
		unsigned int TimeStarted;
		unsigned int Length;
		float tPerc;
		bool tReady;
		bool tComplete;

		
	private:
		
	};
}


#endif