#ifndef IDEF_COOKIE_H
#define IDEF_COOKIE_H

class IDefCookie
{
public:
	virtual ~IDefCookie(){};

	virtual ITexture *GetCookieTarget( const int iTargetIndex ) = 0;
	virtual void PreRender( const int iTargetIndex ){};

	virtual bool IsValid()
	{
		return !IsErrorTexture( GetCookieTarget( 0 ) );
	};
};


#endif