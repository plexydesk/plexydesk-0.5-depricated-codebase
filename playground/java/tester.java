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
import java.io.*;
import java.util.Scanner;
 
 public class tester {
   
    public static void main(String args[]){
        
        Scanner keyboard =new Scanner (System.in);
        circle cc1;
        circle cc2;
        int n=0;
        double x, y, radius;
        
        
        System.out.println("input the x of cc1");
        x=keyboard.nextDouble();
        System.out.println("input the y of cc1");
        y=keyboard.nextDouble();
        System.out.println("input the radius of cc1");
        radius=keyboard.nextDouble();
        
        cc1 = new circle (x, y, radius);
        
        System.out.println("input the x of cc1 frame 2");
        x=keyboard.nextDouble();
        System.out.println("input the y of cc1 frame 2");
        y=keyboard.nextDouble();
        
        cc1.setMovement(x,y);
        
        System.out.println("input the x of cc2");
        x=keyboard.nextDouble();
        System.out.println("input the y of cc2");
        y=keyboard.nextDouble();
        System.out.println("input the radius of cc2");
        radius=keyboard.nextDouble();
        
        cc2 = new circle (x, y, radius);
        
        while (n<3000){    
        
        double pathSlope=(cc1.center.getY()-cc1.movement.getY())/(cc1.center.getX()-cc1.movement.getX());
        double pathB= cc1.center.getY()-(cc1.center.getX()*pathSlope);
        double inversePath= -1.0/pathSlope;
        double inverseB= cc2.center.getY()-(cc2.center.getX()*inversePath);
        double xIntersection= (inverseB+(pathB*-1.0))/(pathSlope + (inversePath*-1.0));
        double yIntersection= (pathSlope*xIntersection)+pathB;
        Point intersector = new Point (xIntersection, yIntersection);
        double hypoLength= Math.sqrt(((cc1.getRadius()+cc2.getRadius())*(cc1.getRadius()+cc2.getRadius()))-(cc2.center.distance(intersector))*cc2.center.distance(intersector));
        double hypoSlopeLength = Math.sqrt((pathSlope*pathSlope)+1);
        double savetime = hypoLength/hypoSlopeLength;
        Point end = new Point((intersector.getX()+(-1.0*savetime)),((intersector.getY())+(-1.0*savetime*pathSlope)));
        
        System.out.println (end.getX()+" "+end.getY());
        
        
        
        
        //the rest of this just increments values so the program has new numbers to calculate for. its basically a test of speed. 
        
        
        
        cc1.setXYRa((cc1.center.getX()+1),cc1.center.getY()+1, cc1.getRadius()+1);
        cc2.setXYRa((cc2.center.getX()+1),cc2.center.getY()+1, cc2.getRadius()+1);
        n++;
        }
        
    }
        
 }
 
        
        
        
       
        
