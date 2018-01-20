#ifndef PROJECTABLE_FACTORY_H
#define PROJECTABLE_FACTORY_H

#include "deferred/vgui/vgui_deferred.h"

typedef CVGUIProjectable* (*pfnGetProjInstance)();

namespace CProjectableFactory
{
	extern bool AddProjectableFactory( pfnGetProjInstance pFn, const char *pszName );
	extern CVGUIProjectable *AllocateProjectableByName( const char *pszName );
	extern CVGUIProjectable *AllocateProjectableByScript( const char *pszFileName );

	class CProjectableManager : public CAutoGameSystemPerFrame
	{
		friend class CVGUIProjectable;
	public:
		CProjectableManager();

		bool Init();
		void Update( float frametime );

	private:
		void RegisterHelpers();

		void AddThinkTarget( CVGUIProjectable *panel );
		void RemoveThinkTarget( CVGUIProjectable *panel );
		CUtlVector< CVGUIProjectable* > m_hInstantiatedPanels;
	};
	extern CProjectableManager *GetProjectableManager();

	class CProjectableHelper
	{
	public:
		static CProjectableHelper *m_pLastHelper;
		CProjectableHelper *m_pNext;

		CProjectableHelper( pfnGetProjInstance pFn, const char *pszName )
		{
			// assumes that we point to a string literal
			m_pszName = pszName;
			m_pFn = pFn;

			m_pNext = m_pLastHelper;
			m_pLastHelper = this;
		};

		const char *m_pszName;
		pfnGetProjInstance m_pFn;
	};
}


#define REGISTER_PROJECTABLE_FACTORY( className, identifierName )\
	static CVGUIProjectable *GetInstanceOf##className()\
	{\
		return new className();\
	};\
	static CProjectableFactory::CProjectableHelper helper##className( GetInstanceOf##className, identifierName );

#endif