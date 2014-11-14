/* -*- Mode: Java; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set shiftwidth=2 tabstop=2 autoindent cindent expandtab: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
"use strict";

const {classes: Cc, interfaces: Ci, utils: Cu, results: Cr} = Components;

Cu.import("resource://gre/modules/XPCOMUtils.jsm");
Cu.import("resource://gre/modules/ctypes.jsm");

const RECOVERYSERVICE_CID = Components.ID("{b3caca5d-0bb0-48c6-912b-6be6cbf08832}");
const RECOVERYSERVICE_CONTRACTID = "@mozilla.org/recovery-service;1";

function log(msg) {
  dump("-*- RecoveryService: " + msg + "\n");
}

//@line 52 "/home/myk/Mozilla/central/b2g/components/RecoveryService.js"

function RecoveryService() {}

RecoveryService.prototype = {
  classID: RECOVERYSERVICE_CID,
  QueryInterface: XPCOMUtils.generateQI([Ci.nsIRecoveryService]),
  classInfo: XPCOMUtils.generateCI({
    classID: RECOVERYSERVICE_CID,
    contractID: RECOVERYSERVICE_CONTRACTID,
    interfaces: [Ci.nsIRecoveryService],
    classDescription: "B2G Recovery Service"
  }),

  factoryReset: function RS_factoryReset() {
//@line 72 "/home/myk/Mozilla/central/b2g/components/RecoveryService.js"
    throw Cr.NS_ERROR_FAILURE;
  },

  installFotaUpdate: function RS_installFotaUpdate(updatePath) {
//@line 82 "/home/myk/Mozilla/central/b2g/components/RecoveryService.js"
    throw Cr.NS_ERROR_FAILURE;
  },

  getFotaUpdateStatus: function RS_getFotaUpdateStatus() {
    let status =  Ci.nsIRecoveryService.FOTA_UPDATE_UNKNOWN;
//@line 95 "/home/myk/Mozilla/central/b2g/components/RecoveryService.js"
    return status;
  }
};

this.NSGetFactory = XPCOMUtils.generateNSGetFactory([RecoveryService]);
