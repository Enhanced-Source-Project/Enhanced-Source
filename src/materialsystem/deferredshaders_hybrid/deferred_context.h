#ifndef DEFERRED_CONTEXT_H
#define DEFERRED_CONTEXT_H

#include "basevsshader.h"

class CDeferredPerMaterialContextData : public CBasePerMaterialContextData
{
public:

	enum DefStage_t
	{
		DEFSTAGE_GBUFFER = 0,
		DEFSTAGE_SHADOW,
		DEFSTAGE_COMPOSITE,
	};

	CDeferredPerMaterialContextData()
	{
		m_iAllocated = -1;
		m_piCommandArray = NULL;
	};

	~CDeferredPerMaterialContextData()
	{
		if ( m_piCommandArray != NULL )
		{
			for ( int i = 0; i <= m_iAllocated; i++ )
				delete [] m_piCommandArray[i];

			delete [] m_piCommandArray;
		}
	};

	void SetCommands( DefStage_t p_iStage, uint8 *p_iCmd )
	{
		if ( p_iStage > m_iAllocated )
		{
			uint8 **pNewCmds = new uint8*[p_iStage + 1];
			Q_memset( pNewCmds, 0, sizeof(uint8*) * (p_iStage + 1) );

			if ( m_piCommandArray != NULL )
			{
				Assert( m_iAllocated >= 0 );

				Q_memcpy( pNewCmds, m_piCommandArray, sizeof(uint8*) * (m_iAllocated + 1) );
				delete [] m_piCommandArray;
			}

			m_piCommandArray = pNewCmds;
			m_iAllocated = p_iStage;
		}

		delete [] m_piCommandArray[ p_iStage ];
		m_piCommandArray[ p_iStage ] = p_iCmd;
	};

	uint8 *GetCommands( DefStage_t p_iStage )
	{
		Assert( m_iAllocated >= p_iStage );

		return m_piCommandArray[ p_iStage ];
	}

	bool HasCommands( DefStage_t p_iStage )
	{
		if ( p_iStage > m_iAllocated )
			return false;
		return m_piCommandArray[ p_iStage ] != NULL;
	}

private:

	int m_iAllocated;

	uint8 **m_piCommandArray;
};



#endif