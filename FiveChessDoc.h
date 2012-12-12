// FiveChessDoc.h : interface of the CFiveChessDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIVECHESSDOC_H__6C8BC09C_E005_4F63_9A87_F4A14DE9547E__INCLUDED_)
#define AFX_FIVECHESSDOC_H__6C8BC09C_E005_4F63_9A87_F4A14DE9547E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFiveChessDoc : public CDocument
{
protected: // create from serialization only
	CFiveChessDoc();
	DECLARE_DYNCREATE(CFiveChessDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFiveChessDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFiveChessDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFiveChessDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIVECHESSDOC_H__6C8BC09C_E005_4F63_9A87_F4A14DE9547E__INCLUDED_)
