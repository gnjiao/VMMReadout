//
//  PropertyTreeParserDetector.cpp
//  mmdaq3-server
//
//  Created by Marcin Byszewski on 30.5.12.
//  Copyright (c) 2012 CERN - PH/ADE. All rights reserved.
//

#include "PropertyTreeParserDetector.h"
#include "DetTypes.h"
#include "DetDetector.h"
#include "DetChamber.h"
#include "DetConnector.h"

#include "SrsTypes.h"
#include "SrsChipId.h"


//#include <boost/foreach.hpp>

#include <iostream>

namespace bpt = boost::property_tree;
using namespace online::display;


CPropertyTreeParserDetector::CPropertyTreeParserDetector(const boost::property_tree::ptree& m_det_cfg, CDaqServerConfig& daqconfig) :
CPropertyTreeParser(m_det_cfg, daqconfig)
{
   
}


CPropertyTreeParserDetector::~CPropertyTreeParserDetector()
{
   
}

/**
 return build detector, connected to srs chips, verified
 */
boost::shared_ptr<CDetBase> CPropertyTreeParserDetector::build_detector()
{
   try {
      boost::shared_ptr<CDetBase> detptr = parse_ptree_det_detector_node(ptree_.get_child("detector"));
      return detptr;
   } catch (std::runtime_error &re) {
      std::cout << "ERROR: CPropertyTreeParserDetector::build_detector failed:\n";
      std::cout << re.what() << std::endl;
      throw;
   }

   return boost::shared_ptr<CDetDetector>();
}


boost::shared_ptr<CDetBase> CPropertyTreeParserDetector::parse_ptree_det_detector_node(const boost::property_tree::ptree& mainnode)
{
   std::string name;
   Coord3 pos;
   Coord3 rot;
   Coord3 size;
   
   boost::shared_ptr<CDetBase> detector(new CDetDetector(name, size, pos, rot));
   
   qDebug("parse_ptree_det_detector_node");
   qDebug()<<"name="<<QString::fromStdString(name);

   std::vector<DetBasePtr> chambers;
   Q_FOREACH(const bpt::ptree::value_type &v, mainnode) {

//       qDebug("v.first == %s",(v.first).c_data());
       qDebug()<<"v.first =="<<QString::fromStdString(v.first);
      if (v.first == "position") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         pos = Coord3(x,y,z);
      }
      else if (v.first == "size") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         size = Coord3(x,y,z);
      }
      else if (v.first == "rotation") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         rot = Coord3(x,y,z);
      }
      else if (v.first == "name") {
         name = v.second.data();
      }
      else if (v.first == "chamber") {
         try {
//              qDebug("Creating chamber === %s",detector);
//              qDebug("Creating chamber === %v",v.first);
            boost::shared_ptr<CDetBase> chamber = parse_ptree_det_chamber_node(v, detector);
            chambers.push_back(chamber);
         } catch (std::exception& e) {
              qDebug(">>>>>>>>>>>>>ERROR creating chamber");
            std::cout << "ERROR creating chamber in " << name << " :\n";
            std::cout << e.what() << std::endl;
         }
         
         
      }
      else if (v.first == "<xmlcomment>") {
         //         std::cout << "comment in file: " << v.second.data() << std::endl;
      }
      else if (v.first == "<xmlattr>") {
         //         std::cout << "<xmlattr>=" << v.second.data() << std::endl;
         name = v.second.get<std::string>("name");
         //         std::cout << "read name=" << name << std::endl;
      }
      else {
         std::cout << "WARN: bad data in detector config: unknown key '" << v.first << "'" << std::endl;
      }
   }
   
   detector->set(detector->idnumber(), name, size, pos, rot);
   
   //TODO: build detector new Det + chambers
//   boost::shared_ptr<CDetBase> detptr(new CDetDetector(name, size, pos, rot));
   size_t ii = 0;
   Q_FOREACH(DetBasePtr cham, chambers) {
      detector->add_child(cham, ii++);
   }
   return detector;
}


boost::shared_ptr<CDetBase> CPropertyTreeParserDetector::parse_ptree_det_chamber_node(const bpt::ptree::value_type& node, boost::shared_ptr<CDetBase> det)
{
   qDebug("parse_ptree_det_chamber_node");
   std::string name;
   std::string config_file;
   Coord3 pos;
   Coord3 rot;
   Coord3 size;
   
   qDebug("going for DetChamPtr");
   DetChamPtr chamber(new CDetChamber(det, det, det->get_children().size(), name, size, pos, rot) );

   qDebug("going for QFOREACH");
   std::vector<DetConnPtr> connectors;
   Q_FOREACH(const bpt::ptree::value_type& v, node.second) {
      if (v.first == "position") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         pos = Coord3(x,y,z);
      }
      else if (v.first == "size") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         size = Coord3(x,y,z);
      }
      else if (v.first == "rotation") {
         double x = v.second.get<double>("x", 0.0);
         double y = v.second.get<double>("y", 0.0);
         double z = v.second.get<double>("z", 0.0);
         rot = Coord3(x,y,z);
      }
      else if (v.first == "name") {
         name = v.second.data();
      }
      else if (v.first == "config_file") {
         config_file = v.second.data();
      }
      else if (v.first == "<xmlcomment>") {
         //         std::cout << "comment in file: " << v.second.data() << std::endl;
      }
      else if (v.first == "<xmlattr>") {
         //         std::cout << "<xmlattr>=" << v.second.data() << std::endl;
         name = v.second.get<std::string>("name");
         //         std::cout << "read name=" << name << std::endl;
      }
//      else {
//         std::cout << "WARN: bad data in detector config: unknown key '" 
//         << v.first << "' in node "   <<  node.first << std::endl;
//      }
   }
   
   chamber->set(chamber->idnumber(), name, size, pos, rot);
   
   boost::filesystem::path config_path(config_file);
   //make absolute paths for included config files
   if (!config_path.has_parent_path()) {
      config_path = daqconfig_.get_config_path().parent_path() / config_path;
   }

   //configure from config file
   chamber->read_config_file(config_path.string(), daqconfig_);
   
   //connector in second step, aftre chamber configured from file
   
   Q_FOREACH(const bpt::ptree::value_type& v, node.second) {
      if (v.first == "connector") {
         try {
              qDebug("Going for conn===");
            DetConnPtr conn = parse_ptree_det_connector_node(v, chamber);
            qDebug("DDD_ALL OK");
         } catch (std::exception& e) {
              qDebug("DDD_THROWING");
            std::cout << "ERROR creating connector in " << node.first << " " << name << ":\n";
            throw;
         }
         
      }
      
   }

   
   Q_FOREACH(DetConnPtr conn, connectors) {
//      std::cout << "TODO:  chamber config: match connectors on chamber to this one (2 instances now!!)" << std::endl;
      //TODO:  match connectors on chamber to this one (2 instances now!!)
      chamber->configure_connector_connection(conn); //will throw if chamber does not have connector
   }
   
   
   return chamber;
}



DetConnPtr CPropertyTreeParserDetector::parse_ptree_det_connector_node(const bpt::ptree::value_type& node,  DetChamPtr chamb)
{
    qDebug("parse_ptree_det_connector_node");
   std::cout << " ===== parse_ptree_det_connector_node() " << std::endl;
   typedef std::pair<int, boost::shared_ptr<CSrsChip> > SeqChipType;
   std::string name;
   std::string connection_map_file;
   std::vector<SeqChipType> chips;
   
   //boost::shared_ptr<CDetConnector> conn(new CDetConnector(chamb->detector(), chamb, 0, name)); 

   
   Q_FOREACH(const bpt::ptree::value_type &v, node.second) {
      if (v.first == "map_file") {
         connection_map_file =  v.second.data();
      }
      else if (v.first == "chip") {
         try {
            std::pair<int, boost::shared_ptr<CSrsChip> > seqchip = parse_ptree_det_chip_node(v);
            qDebug("AAA111");
            chips.push_back(seqchip);
            qDebug("AAA222");
         } catch (std::exception& e) {
            std::cout << "ERROR connecting chip in " << node.first << " " << name << ":\n";
            qDebug("BBB222_EXCEPTION");
            throw;
         }
      }
      else if (v.first == "<xmlcomment>") {
         //         std::cout << "comment in file: " << v.second.data() << std::endl;
      }
      else if (v.first == "<xmlattr>") {
         //         std::cout << "<xmlattr>=" << v.second.data() << std::endl;
         name = v.second.get<std::string>("name");
         //         std::cout << "read name=" << name << std::endl;
      }
      else {
         std::cout << "WARN: bad data in detector config: unknown key '" << v.first << "'"
         << "' in node "   <<  node.first << std::endl;
      }
   }
qDebug("CCC");
   
   
   //set correct path to the map file
   boost::filesystem::path map_path(connection_map_file);
   qDebug("CCC111");
   //make absolute paths for included config files
   if (!map_path.has_parent_path()) {
       qDebug("CCC333333");
      map_path = daqconfig_.get_config_path().parent_path() / map_path;
   }

   qDebug("CCC222");
   if (chips.size() > 1) {
      Q_FOREACH(SeqChipType chip, chips) {
         if (chip.first < 0) {
             qDebug("CCC_THROWING");
               std::stringstream ss;
               ss << "Chip sequence number not specified for " << chip.second->name() <<  " on more than 1 chip connector";
               throw std::range_error(ss.str().c_str());
         }
      }
   }

   qDebug("CCC666");
   boost::shared_ptr<CDetConnector> conn;//(new CDetConnector(chamb->detector(), chamb, 0, name)) ;

   qDebug("CCC777");
   if (boost::shared_ptr<CDetConnector> conn = chamb->get_connector(name)) {
       qDebug("CCC88");
      Q_FOREACH(SeqChipType schip, chips) {
          qDebug("CCC99");
         size_t seq = schip.first<0 ? 0 : schip.first;
         qDebug("CCC_AAA");
         conn->connect_chip(seq, schip.second);
         qDebug("CCC_BBB");
         schip.second->connect_to_connector(conn); 
         qDebug("CCC_CCC");
      }
      conn->read_chip_connection_map_file(map_path.string(), &daqconfig_);
      qDebug("CCC_DFDFDF");
   }
   else {
       qDebug("CCC_THROWING MAAAN");
      std::stringstream ss;
      ss << "CONNECTOR FAIL No connector " << name << " on chamber " << chamb->name();
      throw std::range_error(ss.str().c_str());

   }
      

   qDebug("CCC1000");
   return conn;
   
}



/**
 parse chip connected to detector and look for the chip in the srs elements
 <!-- chip id only -->
 <chip id = "2"></chip>
 
 <!-- chip name only -->
 <chip name = "DBL0"></chip>
 
 <!-- chip id and name -->
 <chip id = "1">DBL1</chip> -- unsupported
 <chip id = "1"> <name>DBL1</name> </chip>
 fec_no can be overspecified too:
 <fec>0</fec>
 */
std::pair<int, boost::shared_ptr<CSrsChip> > CPropertyTreeParserDetector::parse_ptree_det_chip_node(const bpt::ptree::value_type& node)
{
    qDebug("parse_ptree_det_chip_node");
   typedef std::pair<int, boost::shared_ptr<CSrsChip> > SeqChipType;
   
   std::cout << "-----------------------------------------------------------------------" << node.first << std::endl;
   
   std::string idstr;
   std::string namestr;
   std::string fecidstr;
   std::string seqstr;   
   //get chip description



   Q_FOREACH(const boost::property_tree::ptree::value_type &v, node.second) {
       qDebug()<<"v.first    ="<<QString::fromStdString(v.first);
      if (v.first == "name") {
          qDebug("dsads");
          qDebug() << "name ==== " << QString::fromStdString(v.second.get("name",""));
          qDebug("dsads");
         namestr = v.second.data();
      }
      else if (v.first == "id") {
         idstr = v.second.data();
      }
      else if (v.first == "fec") {
         fecidstr = v.second.data();
      }
      else if (v.first == "sequence") {
         seqstr = v.second.data();
      }
      else if (v.first == "<xmlcomment>") {
         //         std::cout << "comment in SRU section: " << v.second.data() << std::endl;
      }
      else if (v.first == "<xmlattr>") {
         std::string idstr2 = v.second.get("id", "");
         std::string fecstr2 = v.second.get("fec", "");
         std::string namestr2 = v.second.get("name", "");
         std::string seqstr2 = v.second.get("sequence", "");
qDebug("in xlmattr");
         if (!idstr2.empty()) {  
            idstr = idstr2;
         }
         if (!fecstr2.empty()) {
            fecidstr = fecstr2;
         }
         if (!namestr2.empty()) {
            namestr = namestr2;
         }
         if (!seqstr2.empty()) {
            seqstr = seqstr2;
         }
      }
      else {
         std::cout << "WARN: bad data in config: unknown key '" << v.first 
         << "' in section " << node.first << std::endl;
      }
   }
   
   qDebug("chip_id = id_number_from_string(idstr)");
   long chip_id = -1;
   if(!idstr.empty()) {
      chip_id = id_number_from_string(idstr);  
   }
   qDebug("22");
   long seq = -1;
   if (!seqstr.empty()) {
      seq = id_number_from_string(seqstr);
   }

   qDebug("33");
   if (!fecidstr.empty()) {
      long fec_id = id_number_from_string(fecidstr);

      qDebug("44");
      //locate specified fec or fail
      SrsFecPtr srsfec = daqconfig_.locate_srs<CSrsFec>(fec_id);
      if (srsfec) {
          qDebug("55");
         boost::shared_ptr<CSrsChip> chip = srsfec->locate_chip(chip_id, namestr);
//         qDebug()<<"chip="<<chip;
         if(!chip) {
             qDebug("66");
            std::stringstream ss;
            qDebug() << "Config error: chip (" << chip_id << " " << QString::fromStdString(namestr)<< ") not found in FEC id=" << fec_id;
            throw std::range_error(ss.str().c_str());
         }
         return SeqChipType(seq, chip);
      }
      else {
          qDebug("77");
         std::stringstream ss;
         ss << "Config error: detector config has FEC id=" << fec_id
         << " that does not exist in srs configuration";
         throw std::range_error(ss.str().c_str());
      }
   }
   else {
       qDebug("88");
      //locate chip in any fec in system  or throw
      SrsChipPtr chip = daqconfig_.locate_srs<CSrsChip>(chip_id, namestr);
      qDebug("99");
      return SeqChipType(seq, chip);
   }
   
   return SeqChipType(-1, boost::shared_ptr<CSrsChip>());
}
