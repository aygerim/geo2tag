#include <iostream>
#include <curl/curl.h>
#include "MapLoaderFactory.h"
#include "GoogleClientLogin.h"

int main(int argc, char **argv)
{
  curl_global_init(CURL_GLOBAL_ALL);
  common::DataMarks marks;
  common::DataMark m;
  m.m_name = 'A';
  m.m_latitude = 60.;
  m.m_longitude = 30.;
  m.m_description = "Hello";
  marks.push_back(m);
  common::Picture p = maps::MapLoaderFactory::getInstance(maps::MapLoader::GOOGLE)->getMapWithMarks(60.,30.,12,800,480,marks);
  //  maps::GoogleClientLogin query("geoblog.hackday2@gmail.com", "geo@blog");
  //  query.login();
  //  std::cerr << query.getAuthToken();

  curl_global_cleanup();
  return 0;
}
