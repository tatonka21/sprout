/**
 * @file bgcfplugin.cpp  Plug-in wrapper for the BGCF Sproutlet.
 *
 * Project Clearwater - IMS in the Cloud
 * Copyright (C) 2014  Metaswitch Networks Ltd
 *
 * Parts of this module were derived from GPL licensed PJSIP sample code
 * with the following copyrights.
 *   Copyright (C) 2008-2011 Teluu Inc. (http://www.teluu.com)
 *   Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version, along with the "Special Exception" for use of
 * the program along with SSL, set forth below. This program is distributed
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * The author can be reached by email at clearwater@metaswitch.com or by
 * post at Metaswitch Networks Ltd, 100 Church St, Enfield EN2 6BQ, UK
 *
 * Special Exception
 * Metaswitch Networks Ltd  grants you permission to copy, modify,
 * propagate, and distribute a work formed by combining OpenSSL with The
 * Software, or a work derivative of such a combination, even if such
 * copying, modification, propagation, or distribution would otherwise
 * violate the terms of the GPL. You must comply with the GPL in all
 * respects for all of the code used other than OpenSSL.
 * "OpenSSL" means OpenSSL toolkit software distributed by the OpenSSL
 * Project and licensed under the OpenSSL Licenses, or a work based on such
 * software and licensed under the OpenSSL Licenses.
 * "OpenSSL Licenses" means the OpenSSL License and Original SSLeay License
 * under which the OpenSSL Project distributes the OpenSSL toolkit software,
 * as those licenses appear in the file LICENSE-OPENSSL.
 */

#include "cfgoptions.h"
#include "bgcfsproutlet.h"

class BGCFPlugin : public SproutletPlugin
{
public:
  BGCFPlugin();
  ~BGCFPlugin();

  std::list<Sproutlet*> load(struct options& opt);
  void unload();

private:
  BGCFSproutlet* _bgcf_sproutlet;
  ACRFactory* _acr_factory;
  BGCFService* _bgcf_service;
};

/// Export the plug-in using the magic symbol "plugin-loader"
BGCFPlugin plugin_loader;


BGCFPlugin::BGCFPlugin() :
  _bgcf_sproutlet(NULL),
  _acr_factory(NULL),
  _scscf_selector(NULL)
{
}

BGCFPlugin::~BGCFPlugin()
{
}

/// Loads the BGCF plug-in, returning the supported Sproutlets.
std::list<Sproutlet*> BGCFPlugin::load(struct options& opt)
{
  std::list<Sproutlet*> sproutlets;

  if (opt.scscf_enabled)
  {
    // Determine the BGCF URIs.
    std::string scscf_uri = std::string(stack_data.scscf_uri.ptr,
                                        stack_data.scscf_uri.slen);
    std::string bgcf_uri = "sip:bgcf." + scscf_uri.substr(4);

    // Create BGCF service required for the BGCF Sproutlet.
    bgcf_service = new BgcfService();

    // Create the BGCF ACR factory.
    acr_factory = (ralf_connection != NULL) ?
                       (ACRFactory*)new RalfACRFactory(ralf_connection, BGCF) :
                       new ACRFactory();

    // Create the Sproutlet.
    bgcf_sproutlet = new BGCFSproutlet(0, bgcf_service, acr_factory);

    sproutlets.push_back(bgcf_sproutlet);
  }

  return sproutlets;
}

/// Unloads the BGCF plug-in.
void BGCFPlugin::unload()
{
  delete bgcf_sproutlet;
  delete acr_factory;
  delete bgcf_service;
}
