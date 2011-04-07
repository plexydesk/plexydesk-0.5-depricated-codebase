/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Sri Lanka Institute of Information Technology
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/


#include <QXmlQuery>
#include "weatherplugin.h"
#include <QBuffer>
#include <QTest>
#include <QDebug>
using namespace PlexyDesk;


WeatherPlugin::WeatherPlugin(QObject *parent) : SocialPlugin(parent)
{
    WEATHER_FROM_CITYCODE_URL = QUrl("http://www.google.com/ig/api?weather=colombo");

    job = new HttpJobHandler(this);
    connect(job, SIGNAL(finished()), this, SLOT(listen()));
}



void WeatherPlugin::listen()
{
    if (job->isError())
    {
        weatherError = job->errorName();
    }
    else
    {
        weatherXml = job->readData();
    }
}
void WeatherPlugin::getXml(QUrl &url)
{
    job->getFile(url);

}



QVariantMap WeatherPlugin::getweather()
{

    if (FLAG == 0)
        getXml(WEATHER_FROM_CITYCODE_URL);
    else if (FLAG == 1)
        getXml(WEATHER_FROM_GIVEN_STATION_URL);

    QBuffer buffer;
    while (weatherXml.size() == 0) {
        QTest::qWait(1000);
    }

    buffer.setBuffer(&weatherXml);
    buffer.open(QIODevice::ReadOnly);

    QXmlQuery query;
    query.setFocus(&buffer);

    QStringList temp, humidity, windspeed, current, forecastDate, forecastDay, forecastLowTemp, forecastHighTemp, forecastCondition, image, f_image, c_date;

    query.setQuery("for $x in //xml_api_reply/weather/current_conditions  return string($x /temp_c/@data)");
    query.evaluateTo(&temp);

    query.setQuery("for $x in //xml_api_reply/weather/current_conditions  return string($x /humidity/@data)");
    query.evaluateTo(&humidity);

    query.setQuery("for $x in //xml_api_reply/weather/current_conditions  return string($x /wind_condition/@data)");
    query.evaluateTo(&windspeed);

    query.setQuery("for $x in //xml_api_reply/weather/current_conditions  return string($x /condition/@data)");
    query.evaluateTo(&current);

    query.setQuery("for $x in //xml_api_reply/weather/forecast_information  return string($x /forecast_date/@data)");
    query.evaluateTo(&forecastDate);

    query.setQuery("for $x in //xml_api_reply/weather/forecast_conditions  return string($x /day_of_week/@data)");
    query.evaluateTo(&forecastDay);

    query.setQuery("for $x in //xml_api_reply/weather/forecast_conditions  return string($x /low/@data)");
    query.evaluateTo(&forecastLowTemp);

    query.setQuery("for $x in //xml_api_reply/weather/forecast_conditions  return string($x /high/@data)");
    query.evaluateTo(&forecastHighTemp);

    query.setQuery("for $x in //xml_api_reply/weather/forecast_conditions  return string($x /condition/@data)");
    query.evaluateTo(&forecastCondition);

    query.setQuery("for $x in //xml_api_reply/weather/current_conditions  return string($x /icon/@data)");
    query.evaluateTo(&image);

    query.setQuery("for $x in //xml_api_reply/weather/forecast_conditions  return string($x /icon/@data)");
    query.evaluateTo(&f_image);

    query.setQuery("for $x in //xml_api_reply/weather/forecast_information  return string($x /current_date_time/@data)");
    query.evaluateTo(&c_date);


    weatherXml.clear();

    QString imgName = image.at(0);
    image.clear();
    imgName = imgName.remove(0, imgName.lastIndexOf("/")+1);
    imgName.truncate(imgName.lastIndexOf("."));
    image<<imgName;
    QStringList tmp;
    for (int i = 0; i<f_image.length(); i++)
    {
        imgName = f_image.at(i);
        imgName = imgName.remove(0, imgName.lastIndexOf("/")+1);
        imgName.truncate(imgName.lastIndexOf("."));
        tmp<<imgName;
    }
    f_image.clear();
    f_image = tmp;

    imgName = c_date.at(0);
    imgName.truncate(imgName.indexOf(" "));
    c_date.clear();
    c_date<<imgName;

    QVariantMap result;
    result["z1f_Day"] = QVariant(forecastDay);
    result["z3f_LowTemp"] = QVariant(forecastLowTemp);
    result["z4f_HighTemp"] = QVariant(forecastHighTemp);
    result["z2f_Condition"] = QVariant(forecastCondition);
    result["temp"] = QVariant(temp);
    result["humidity"] = QVariant(humidity);
    result["wind"] = QVariant(windspeed);
    result["current"] = QVariant(current);
    result["z5curr_image"] = QVariant(image);
    result["z6fore_image"] = QVariant(f_image);
    result["z7curr_date"] = QVariant(c_date);
    return result;
}




QVariantMap WeatherPlugin::data(const QString &methodName, QVariantMap args)
{
/*
    QVariant station = args["station"];
    if (station.isValid())
    {
        //TODO : args decoding does not work properly
        WEATHER_FROM_GIVEN_STATION_URL = "http://www.google.com/ig/api?weather="+station.value<QString>();
        FLAG = 1;
    }else*/
    FLAG = 0;

    return this->getweather();
}

QStringList WeatherPlugin::supportedMethods()
{
    // qDebug()<<"WeatherPlugin::supportedMethods() called";
    QStringList list;
    list <<"getweather";
    return list;
}
