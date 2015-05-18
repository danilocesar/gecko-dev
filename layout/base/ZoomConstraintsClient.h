/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef ZoomConstraintsClient_h_
#define ZoomConstraintsClient_h_

#include "nsIDOMEventListener.h"
#include "nsIDocument.h"
#include "nsIObserver.h"

#include "FrameMetrics.h"


class nsIDocument;
class PresShell;

class ZoomConstraintsClient final : public nsIDOMEventListener,
                                    public nsIObserver
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMEVENTLISTENER
  NS_DECL_NSIOBSERVER

  ZoomConstraintsClient();

private:
  ~ZoomConstraintsClient();

public:
  void Init(PresShell* aPresShell, nsIDocument *aDocument);
  void Destroy();

private:
  void RefreshZoomConstraints();
  nsCOMPtr<nsIDocument> mDocument;
  nsRefPtr<PresShell>   mPresShell;
};

#endif

