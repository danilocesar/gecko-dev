/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ZoomConstraintsClient.h"

#include "FrameMetrics.h"
#include "mozilla/layers/APZCCallbackHelper.h"
#include "nsDocument.h"
#include "nsIFrame.h"
#include "nsLayoutUtils.h"
#include "nsPoint.h"
#include "nsPresShell.h"
#include "nsView.h"
#include "nsViewportInfo.h"
#include "Units.h"

NS_IMPL_ISUPPORTS(ZoomConstraintsClient, nsIDOMEventListener, nsIObserver)

#define DOMMETADATA "DOMMetaAdded"
static const char BEFORE_FIRST_PAINT[] = "before-first-paint";

using namespace mozilla;
using namespace mozilla::layers;

ZoomConstraintsClient::ZoomConstraintsClient() :
  mDocument(nullptr),
  mPresShell(nullptr)
{
}

ZoomConstraintsClient::~ZoomConstraintsClient()
{
}

void
ZoomConstraintsClient::Destroy()
{
  if (!(mPresShell && mDocument)) {
    return;
  }

  mDocument->RemoveEventListener(NS_LITERAL_STRING(DOMMETADATA), this, false);

  nsCOMPtr<nsIObserverService> observerService = mozilla::services::GetObserverService();
  if (observerService) {
    observerService->RemoveObserver(this, BEFORE_FIRST_PAINT);
  }

  mDocument = nullptr;
  mPresShell = nullptr;
}

void
ZoomConstraintsClient::Init(PresShell* aPresShell, nsIDocument *aDocument)
{
  if (!(aPresShell && aDocument)) {
    return;
  }

  mPresShell = aPresShell;
  mDocument = aDocument;

  mDocument->AddEventListener(NS_LITERAL_STRING(DOMMETADATA),
                this,
                false, false, 0);

  nsCOMPtr<nsIObserverService> observerService = mozilla::services::GetObserverService();
  if (observerService) {
    observerService->AddObserver(this, BEFORE_FIRST_PAINT, false);
  }
}

NS_IMETHODIMP ZoomConstraintsClient::HandleEvent(nsIDOMEvent *event)
{
  RefreshZoomConstraints();
  return NS_OK;
}

NS_IMETHODIMP ZoomConstraintsClient::Observe(nsISupports *aSubject, const char * aTopic, const char16_t * aData)
{
    if (aSubject == mDocument)
        RefreshZoomConstraints();
    return NS_OK;
}

void
ZoomConstraintsClient::RefreshZoomConstraints()
{
  nsIFrame *rootFrame = mPresShell->GetRootFrame();
  if (!rootFrame) {
    return;
  }

  nsSize size = nsLayoutUtils::CalculateCompositionSizeForFrame(rootFrame, false);
  int32_t auPerDevPixel = mPresShell->GetPresContext()->AppUnitsPerDevPixel();
  ScreenIntSize screenSize = ScreenIntSize(size.width / auPerDevPixel, size.height / auPerDevPixel);

  nsViewportInfo viewportInfo =
    nsContentUtils::GetViewportInfo(mDocument, ScreenIntSize(screenSize.width, screenSize.height));

  mozilla::layers::ZoomConstraints zoomConstraints;

  zoomConstraints.mAllowZoom = viewportInfo.IsZoomAllowed();
  zoomConstraints.mAllowDoubleTapZoom = viewportInfo.IsDoubleTapZoomAllowed();
  zoomConstraints.mMinZoom.scale = viewportInfo.GetMinZoom().scale;
  zoomConstraints.mMaxZoom.scale = viewportInfo.GetMaxZoom().scale;

  uint32_t presShellId = 0;
  mozilla::layers::FrameMetrics::ViewID viewId = FrameMetrics::NULL_SCROLL_ID;

  bool scrollIdentifiersValid = APZCCallbackHelper::GetOrCreateScrollIdentifiers(
        mDocument->GetDocumentElement(),
        &presShellId, &viewId);

  if (scrollIdentifiersValid && rootFrame->GetView()) {
    nsIWidget *widget = rootFrame->GetView()->GetWidget();
    if(widget) {
      widget->UpdateZoomConstraints(zoomConstraints, presShellId, viewId);
    }
  }
}
