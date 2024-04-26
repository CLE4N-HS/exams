#pragma once
#include "tools.h"

 typedef struct 
{
	sfVector2i defaultVideoMode;
	sfVideoMode defaultDesktopMode;
	sfVector2f ratioResolution;
	sfFloatRect rectViewPort;
	sfView* view;
	sfVector2f PosView;
	sfFloatRect rectView;
}View;

View* mainView;

/// <summary>
/// Gets the main View of the project
/// </summary>
/// <returns>The main View</returns>
sfView* getView();

/// <summary>
/// Creates a view and configures it directly
/// </summary>
/// <param name="_defaultVideoMode">set a default VideoMode, default (covers the entire screen) -> {1920, 1080, 32} , "32" is the VideoMode pixel depth (in bits per pixels)</param>
/// <param name="_rectViewPort">Viewport rect if needed, default (covers the entire screen) -> {0.f, 0.f, 1.f, 1.f}</param>
/// <param name="_defaultDesktopMode">set a default Desktop mode, default (covers the entire screen) -> {1920, 1080}</param>
/// <returns>The structure of the created View</returns>
View* setupView(sfVector2i _defaultVideoMode, sfFloatRect _rectViewPort, sfVideoMode _defaultDesktopMode);

/// <summary>
/// Creates a view and configures it directly, also differentiates the View and the Viewport
/// </summary>
/// <param name="_defaultVideoMode">set a default VideoMode, default (covers the entire screen) -> {1920, 1080, 32} , "32" is the VideoMode pixel depth (in bits per pixels)</param>
/// <param name="_rectViewPort">Viewport rect if needed, default (covers the entire screen) -> {0.f, 0.f, 1.f, 1.f}</param>
/// <param name="_defaultDesktopMode">set a default Desktop mode, default (covers the entire screen) -> {1920, 1080}</param>
/// <param name="_rectViewPort">View rect (center and scale), default (centred on the entire screen) -> {960.f, 540.f, 1920.f, 1080.f}</param>
/// <returns>The structure of the created View</returns>
View* setupViewCustom(sfVector2i _defaultVideoMode, sfFloatRect _rectViewPort, sfVideoMode _defaultDesktopMode, sfFloatRect _rectView);

/// <summary>
/// Sets and overwrites the rect of a View
/// "</summary>
/// <param name="_view">The whole structure of the View</param>
/// <param name="_rectview">The new rect</param>
void setRectView(View* _view, sfFloatRect _rectview);

/// <summary>
/// Gets the current position of a View, warning : the position of a View is always in the center
/// </summary>
/// <param name="_view">The whole structure of the View</param>
/// <returns>The position of the View in global coordinates</returns>
sfVector2f GetViewPosition(View* _view);

/// <summary>
/// Sets and overwrites the View position
/// </summary>
/// <param name="_view">The whole structure of the View</param>
/// <param name="_position">The new position in global coordinates</param>
void SetViewPosition(View* _view, sfVector2f _position);
