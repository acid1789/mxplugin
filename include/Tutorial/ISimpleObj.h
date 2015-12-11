class ISimpleObj 								
{
public:
	virtual const char* GetMsg()= 0;		
	virtual void		Delete() = 0;
};