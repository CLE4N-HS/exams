#include "viewManager.h"



sfView* getView()
{
	return mainView->view;
}

View* setupView(sfVector2i _defaultVideoMode, sfFloatRect _rectViewPort, sfVideoMode _defaultDesktopMode)
{
	View* tmp = (View*)malloc(sizeof(View));
	tmp->defaultDesktopMode = _defaultDesktopMode;
	tmp->defaultVideoMode = _defaultVideoMode;
	tmp->ratioResolution = (sfVector2f){ (float)tmp->defaultDesktopMode.width / (float)tmp->defaultVideoMode.x, (float)tmp->defaultDesktopMode.height / (float)tmp->defaultVideoMode.y };
	tmp->rectViewPort = _rectViewPort;
	tmp->view = sfView_create();
	tmp->PosView = (sfVector2f){ tmp->defaultVideoMode.x / 2 * tmp->rectViewPort.width, tmp->defaultVideoMode.y / 2 * tmp->rectViewPort.height };
	tmp->rectView = (sfFloatRect){ (float)tmp->defaultDesktopMode.width / 2 , (float)tmp->defaultDesktopMode.height / 2, (float)tmp->defaultDesktopMode.width / tmp->ratioResolution.x * tmp->rectViewPort.width, (float)tmp->defaultDesktopMode.height / tmp->ratioResolution.y * tmp->rectViewPort.height };
	sfView_reset(tmp->view, tmp->rectView);
	sfView_setViewport(tmp->view, tmp->rectViewPort);
	return tmp;
}


View* setupViewCustom(sfVector2i _defaultVideoMode, sfFloatRect _rectViewPort, sfVideoMode _defaultDesktopMode, sfFloatRect _rectView)
{
	View* tmp = (View*)malloc(sizeof(View));
	tmp->defaultDesktopMode = _defaultDesktopMode;
	tmp->defaultVideoMode = _defaultVideoMode;
	tmp->ratioResolution = (sfVector2f){ (float)tmp->defaultDesktopMode.width / (float)tmp->defaultVideoMode.x, (float)tmp->defaultDesktopMode.height / (float)tmp->defaultVideoMode.y };
	tmp->rectViewPort = _rectViewPort;
	tmp->view = sfView_create();
	tmp->PosView = (sfVector2f){ tmp->defaultVideoMode.x / 2 * tmp->rectViewPort.width, tmp->defaultVideoMode.y / 2 * tmp->rectViewPort.height };
	tmp->rectView = _rectView;
	sfView_reset(tmp->view, tmp->rectView);
	sfView_setViewport(tmp->view, tmp->rectViewPort);
	return tmp;
}

void setRectView(View* _view, sfFloatRect _rectview)
{
	_view->rectView = _rectview;
	sfView_reset(_view->view, _view->rectView);
}

sfVector2f GetViewPosition(View* _view)
{
	return _view->PosView;
}

void SetViewPosition(View* _view, sfVector2f _position)
{
	_view->PosView = _position;
	sfView_setCenter(_view->view, _view->PosView);
}