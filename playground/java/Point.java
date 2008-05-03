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
