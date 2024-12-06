/*! \file RotoScopeDoc.h
 *
 * Header file for the RotoScope document class
 * \author Charles B. Owen
 */

#pragma once

#include <vector>
#include <list>
#include <stack>

#include "video/MovieSource.h"
#include "video/MovieMake.h"
#include "audio/DirSoundSource.h"
#include "graphics/GrImage.h"
#include "LineDlg.h"
#include "ArmDlg.h"
#include "CPaintEffect.h"

/*! \mainpage RotoScope Starter Appliaidanion
 *
 * The Rotoscope appliaidanion is a simple program that allows you to 
 * load frames of video, modify them in interesting ways, and save 
 * them to a new video file. Rotoscoping is an animation technique where 
 * frames of a movie are advanced one by one and edited by a user. 
 */

//! Rotoscope document class 
/*! RotoScope document class */
class CRotoScopeDoc : public CDocument
{
public:
	CRotoScopeDoc();
	virtual ~CRotoScopeDoc();
	DECLARE_DYNCREATE(CRotoScopeDoc)

	virtual BOOL OnNewDocument();
    void Mouse(int p_x, int p_y);

    /*! Get the image we are editing 
     * \returns Constant pointer to image we are editing */
    const CGrImage &GetImage() const {return m_image;}

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

private:
    afx_msg void OnMoviesOpensourcemovie();
    afx_msg void OnMoviesOpenoutputmovie();
    afx_msg void OnFramesCreateoneframe();

    void CreateOneFrame();

    CMovieSource        m_moviesource;      // Any source movie
    CMovieMake          m_moviemake;        // Any movie we are outputing to
    CDirSoundSource     m_backaudio;        // Any background audio?

    CGrImage            m_image;            // Current image being displayed
    std::vector<short>  m_audio;            // Current audio to be written to the output
	int					m_movieframe;       // Movie frame we are writing
	std::vector<std::list<CPoint> >     m_draw;
	CGrImage			m_initial;
	CLineDlg			m_dlg;
	armDlg				m_armDlg;
	CGrImage			m_bird;
	CGrImage			m_mario;
	
	
	CGrImage			m_aidan;
	CGrImage			m_aidanArm;
	CGrImage			rotatedArm;
	CGrImage			m_chromaKeyBackground; // Shared background for green/blue screen

	CGrImage			m_julia;

	CGrImage			m_fireworks;

	CGrImage			m_trevor;	

	CGrImage			m_greg;

	CPaintEffect		m_paintEffect;

	int m_x1;
	int m_y1;
	int m_x2;
	int m_y2;
	int m_b;
	int m_g;
	int m_r;
	int m_width;
	int m_theta;

	int m_mode;
	int m_dot_count;

	bool m_morphEnabled;
	bool m_waveEnabled;
	CGrImage m_tempImage;
	bool m_greenScreenEnabled;
	bool m_blueScreenEnabled;

	std::stack<CGrImage>	m_images;
	bool m_applyWaveEffect;

    afx_msg void OnFramesWriteoneframe();
    afx_msg void OnUpdateFramesWriteoneframe(CCmdUI *pCmdUI);
    afx_msg void OnMoviesClosesourcemovie();
    afx_msg void OnUpdateMoviesClosesourcemovie(CCmdUI *pCmdUI);
    afx_msg void OnMoviesCloseoutputmovie();
    afx_msg void OnUpdateMoviesCloseoutputmovie(CCmdUI *pCmdUI);
    afx_msg void OnFramesWritethencreateoneframe();
    afx_msg void OnUpdateFramesWritethencreateoneframe(CCmdUI *pCmdUI);
    afx_msg void OnFramesWritethencreateonesecond();
    void MessagePump(void);
    afx_msg void OnFramesWritethencreateremaining();
    afx_msg void OnMoviesOpenbackgroundaudio();
    afx_msg void OnMoviesClosebackgroundaudio();
    afx_msg void OnUpdateMoviesClosebackgroundaudio(CCmdUI *pCmdUI);
public:
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void SaveMovieData(IXMLDOMDocument *xmlDoc, IXMLDOMNode *inNode);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void XmlLoadMovie(IXMLDOMNode *xml);
	void XmlLoadFrame(IXMLDOMNode *xml);
	afx_msg void OnEditClearframe();
	void DrawImage();
	void DrawLine(CGrImage &image, int x1, int y1, int x2, int y2, int width, int b, int g, int r);
	afx_msg void OnEditDrawline();
	afx_msg void OnEditPlacebird();
	void DrawBird(CGrImage &image, int x1, int y1);
	void CRotoScopeDoc::RotateImage(CGrImage& image, int theta);
	afx_msg void OnEditRotateimage();
	afx_msg void OnEditSetvariables();
	afx_msg void OnMousemodePen();
	afx_msg void OnMousemodeLine();
	afx_msg void OnMousemodeBird();
	afx_msg void OnEditUndo32793();

	void ApplyWaveEffect(); // julia
	void CRotoScopeDoc::DrawJulia(CGrImage& image, int x1, int y1);
	void CRotoScopeDoc::RecolorRedToBlue(CGrImage& inputImage, CGrImage& outputImage);
	
	//Aidan
	void CRotoScopeDoc::Chromakey(CGrImage& foreground, CGrImage& background, CGrImage& output, CGrImage& garbageMask); 
	void CRotoScopeDoc::DrawAidan(CGrImage& image, int bodyX, int bodyY);
	afx_msg void OnMousemodeAidan(); 
	void CRotoScopeDoc::RotateArm(double angle, CGrImage& rotatedArm);
	afx_msg void OnEditBluescreen();
	afx_msg void OnEditChoosebackground();
	void CRotoScopeDoc::OnUpdateEditBluescreen(CCmdUI* pCmdUI);
	


	void CRotoScopeDoc::DrawMario(CGrImage& image, int x1, int y1); //Trevor

	void CRotoScopeDoc::DrawTrevor(CGrImage& image, int x1, int y1); //Trevor

	void CRotoScopeDoc::DrawGreg(CGrImage& image, int x1, int y1); //Greg
	afx_msg void OnEditPlacemario(); //Trevor

	afx_msg void OnMousemodeMario();
	afx_msg void OnMousemodeJulia();

	void CRotoScopeDoc::DrawFireworks(CGrImage& image, int x1, int y1); //Trevor

	afx_msg void OnMousemodeTrevor();
	afx_msg void OnMousemodeApplywavewarp();
	afx_msg void OnEditPlaceaidan();
	afx_msg void OnMousemodeGreg();
	afx_msg void OnEditMorph();
	afx_msg void OnUpdateEditMorph(CCmdUI* pCmdUI);
	afx_msg void OnMousemodeRecolor();
	afx_msg void OnUpdateMousemodeApplywavewarp(CCmdUI* pCmdUI);
	afx_msg void OnEditGreenscreen();
	afx_msg void OnUpdateEditGreenscreen(CCmdUI* pCmdUI);
	afx_msg void OnMousemodePaint();
	afx_msg void OnEditPaintsettings();

};


