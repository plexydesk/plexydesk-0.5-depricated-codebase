import java.io.*;
import java.util.Scanner;
 
 public class circle {
    
    Point center = new Point (0,0);
    Point movement = new Point (0,0);
    double radius;
    
    public circle(double x, double y, double radiu){ 
        center.setX(x);
        center.setY(y);
        radius=radiu;
    }
    
    public void setMovement(double x, double y){
        movement.setX(x);
        movement.setY(y);
    }
    
    public void setXYRa(double x, double y, double radiu){
        center.setX(x);
        center.setY(y);
        radius=radiu;
    
    }
    
    public double getRadius(){
        return radius;
    }
 }
        
