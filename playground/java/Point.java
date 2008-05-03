/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
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
public class Point {
    private double x;
    private double y;

   
    public Point(double xValue, double yValue) {
        this.x = xValue;
        this.y = yValue;
    }

    
    public Point()  {
        x = 0.0;
        y = 0.0;
    }

   
    public Point(Point p) {
		this(p.x, p.y);
    }
    
    public double distance(Point p) {
        return (Math.sqrt((this.x - p.x) * (this.x - p.x) +
                (this.y - p.y) * (this.y - p.y)));
    }

    public double getX()  {
        return x;
    }

   
    public double getY() {
        return y;
    }

    
    public void setX(double xVal) {
		x = xVal;
    }

  
	public void setY(double yVal) {
		y = yVal;
    }
  
      
}
