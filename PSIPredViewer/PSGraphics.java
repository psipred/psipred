
import java.awt.*;
import java.awt.image.ImageObserver;
import java.awt.image.PixelGrabber;
import java.io.*;
import java.text.*;

public class PSGraphics extends Graphics {

    public static final boolean DEBUG = true;
    protected static final int PAGEHEIGHT = 846;
    protected static final int PAGEWIDTH = 594;
    protected static final int XOFFSET = 30;
    protected static final int YOFFSET = 30;
    protected static final char hd[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F'
    };
    protected static final int charsPerRow = 72;
    protected PrintWriter os;
    protected Color clr;
    protected Color backClr;
    protected String fontName;
    protected int fontSize;
    protected Rectangle clippingRect;

    public PSGraphics()
    throws IOException
    {
        this(((Writer) (new OutputStreamWriter(System.out))), true);
	//try
	//{
	//this(((Writer) (new OutputStreamWriter(new FileOutputStream(new File(outputfile))))), true);
    	//}
	//catch( IOException e )
	//{
	//	System.err.println( e );
	//}
    }

    public PSGraphics(Writer w) {
        this(w, true);
    }

    public PSGraphics(Writer w, boolean emitProlog) {
        clr = Color.black;
        backClr = Color.white;
        fontName = "Courier";
        fontSize = 12;
        clippingRect = new Rectangle(0, 0, 594, 846);
        setOutput(w);
        if(emitProlog)
            emitProlog();
    }

    public Graphics create() {
        throw new RuntimeException("create() not implemented");
    }

    public void setOutput(Writer w) {
        if(!(w instanceof PrintWriter))
            os = new PrintWriter(w, true);
        else
            os = (PrintWriter)w;
    }

    public void translate(int x, int y) {
        os.print(x);
        os.print(" ");
        os.print(y);
        os.println(" translate");
    }

    protected void scale(double x, double y) {
        os.print(x);
        os.print(" ");
        os.print(y);
        os.println(" scale");
    }

    protected void lineto(int x, int y) {
        os.print(x);
        os.print(" ");
        os.print(y);
        os.println(" lineto");
    }

    protected void moveto(int x, int y) {
        os.print(x);
        os.print(" ");
        os.print(y);
        os.println(" moveto");
    }

    public Color getColor() {
        diagnostic("getColor()");
        return clr;
    }

    public void setBackground(Color c) {
        diagnostic("setBackground(" + c + ")");
        backClr = c;
    }

    public void setColor(Color c) {
        diagnostic("setColor(" + c + ")");
        if(c != null)
            clr = c;
        os.print((double)clr.getRed() / 255D);
        os.print(" ");
        os.print((double)clr.getGreen() / 255D);
        os.print(" ");
        os.print((double)clr.getBlue() / 255D);
        os.println(" setrgbcolor");
    }

    public void setPaintMode() {
        diagnostic("setPaintMode()");
    }

    public void setXORMode(Color c1) {
        diagnostic("setXORMode(" + c1 + ")");
    }

    public Font getFont() {
        diagnostic("getFont()");
        return null;
    }

    public void setFont(Font f) {
        diagnostic("setFont(" + f + ")");
    }

    public void setFontNameSize(String name, int size) {
        diagnostic("setFontNameSize(" + fontName + fontSize + ")");
        fontName = name;
        fontSize = size;
        os.println("/" + fontName + " findfont");
        os.println(fontSize + " scalefont setfont");
    }

    public FontMetrics getFontMetrics() {
        diagnostic("getFontMetrics()");
        return null;
    }

    public int getFontHeight() {
        return fontSize;
    }

    public float getFontWidth() {
        return (float)(0.59999999999999998D * (double)fontSize);
    }

    public FontMetrics getFontMetrics(Font f) {
        diagnostic("getFontMetrics(" + f + ")");
        return Toolkit.getDefaultToolkit().getFontMetrics(f);
    }

    public Rectangle getClipRect() {
        diagnostic("getClipRect()");
        return clippingRect;
    }

    public Shape getClip() {
        diagnostic("getClip()");
        return clippingRect;
    }

    public void setClip(Shape s) {
        diagnostic("setClip(" + s + ")");
        Rectangle r = s.getBounds();
        setClip(r.x, r.y, r.width, r.height);
    }

    public Rectangle getClipBounds() {
        diagnostic("getClipBounds()");
        return clippingRect;
    }

    public void setClip(int x, int y, int width, int height) {
        diagnostic("setClip(" + x + ", " + y + ", " + width + ", " + height + ")");
        y = transformY(y);
        clippingRect = new Rectangle(x, y, width, height);
        os.println("initclip");
        moveto(x, y);
        lineto(x + width, y);
        lineto(x + width, y - height);
        lineto(x, y - height);
        os.println("closepath eoclip newpath");
    }

    public void clipRect(int x, int y, int width, int height) {
        diagnostic("clipRect(" + x + ", " + y + ", " + width + ", " + height + ")");
        setClip(x, y, width, height);
    }

    public void copyArea(int x, int y, int width, int height, int dx, int dy) {
        diagnostic("copyArea(" + x + ", " + y + ", " + width + ", " + height + ", " + dx + ", " + dy + ")");
        throw new RuntimeException("copyArea not supported");
    }

    public void drawLine(int x1, int y1, int x2, int y2) {
        diagnostic("drawLine(" + x1 + ", " + y1 + ", " + x2 + ", " + y2 + ")");
        y1 = transformY(y1);
        y2 = transformY(y2);
        moveto(x1, y1);
        lineto(x2, y2);
        stroke(false);
    }

    protected void doRect(int x, int y, int width, int height, boolean fill) {
        diagnostic("doRect(" + x + ", " + y + ", " + width + ", " + height + ", " + fill + ")");
        y = transformY(y);
        moveto(x, y);
        lineto(x + width, y);
        lineto(x + width, y - height);
        lineto(x, y - height);
        lineto(x, y);
        stroke(fill);
    }

    public void fillRect(int x, int y, int width, int height) {
        diagnostic("fillRect(" + x + ", " + y + ", " + width + ", " + height + ")");
        doRect(x, y, width, height, true);
    }

    public void drawRect(int x, int y, int width, int height) {
        diagnostic("drawRect(" + x + ", " + y + ", " + width + ", " + height + ")");
        doRect(x, y, width, height, false);
    }

    public void clearRect(int x, int y, int width, int height) {
        diagnostic("clearRect(" + x + ", " + y + ", " + width + ", " + height + ")");
        gsave();
        Color c = getColor();
        setColor(backClr);
        doRect(x, y, width, height, true);
        setColor(c);
        grestore();
    }

    protected void doRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight, boolean fill) {
        diagnostic("doRoundRect(" + x + ", " + y + ", " + width + ", " + height + arcWidth + ", " + arcHeight + ", " + fill + ")");
        y = transformY(y);
        gsave();
        int arcDim = arcHeight / 2;
        translate(x, y);
        if(arcHeight != arcWidth)
            if(arcHeight > arcWidth) {
                double ratio = (double)arcHeight / (double)arcWidth;
                scale(1.0D, ratio);
                height = (int)((double)height / ratio);
                arcDim = arcWidth / 2;
            } else {
                double ratio = (double)arcWidth / (double)arcHeight;
                scale(ratio, 1.0D);
                width = (int)((double)width / ratio);
                arcDim = arcHeight / 2;
            }
        os.println("0 setlinewidth");
        moveto(arcDim, 0);
        arcTo(width, 0, width, -height, arcDim);
        arcTo(width, -height, 0, -height, arcDim);
        arcTo(0, -height, 0, 0, arcDim);
        arcTo(0, 0, width, 0, arcDim);
        stroke(fill);
        os.println("1 setlinewidth");
        grestore();
    }

    protected void stroke(boolean fill) {
        if(fill) {
            gsave();
            os.println("eofill");
            grestore();
        }
        os.println("stroke");
    }

    protected void arcTo(int x1, int y1, int x2, int y2, int dim) {
        os.print(x1);
        os.print(" ");
        os.print(y1);
        os.print(" ");
        os.print(x2);
        os.print(" ");
        os.print(y2);
        os.print(" ");
        os.print(dim);
        os.println(" arcto");
        os.println("4 {pop} repeat");
    }

    public void drawRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight) {
        diagnostic("drawRoundRect(" + x + ", " + y + ", " + width + ", " + height + ", " + arcWidth + ", " + arcHeight + ")");
        doRoundRect(x, y, width, height, arcWidth, arcHeight, false);
    }

    public void fillRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight) {
        diagnostic("fillRoundRect(" + x + ", " + y + ", " + width + ", " + height + ", " + arcWidth + ", " + arcHeight + ")");
        doRoundRect(x, y, width, height, arcWidth, arcHeight, true);
    }

    public void draw3DRect(int x, int y, int width, int height, boolean raised) {
        diagnostic("draw3DRect(" + x + ", " + y + ", " + width + ", " + height + ", " + raised + ")");
        Color c = getColor();
        Color brighter = c.brighter();
        Color darker = c.darker();
        setColor(raised ? brighter : darker);
        drawLine(x, y, x, y + height);
        drawLine(x + 1, y, (x + width) - 1, y);
        setColor(raised ? darker : brighter);
        drawLine(x + 1, y + height, x + width, y + height);
        drawLine(x + width, y, x + width, y + height);
        setColor(c);
    }

    public void fill3DRect(int x, int y, int width, int height, boolean raised) {
        diagnostic("fill3DRect(" + x + ", " + y + ", " + width + ", " + height + ", " + raised + ")");
        Color c = getColor();
        Color brighter = c.brighter();
        Color darker = c.darker();
        if(!raised)
            setColor(darker);
        fillRect(x + 1, y + 1, width - 2, height - 2);
        setColor(raised ? brighter : darker);
        drawLine(x, y, x, (y + height) - 1);
        drawLine(x + 1, y, (x + width) - 2, y);
        setColor(raised ? darker : brighter);
        drawLine(x + 1, (y + height) - 1, (x + width) - 1, (y + height) - 1);
        drawLine((x + width) - 1, y, (x + width) - 1, (y + height) - 1);
        setColor(c);
    }

    public void drawOval(int x, int y, int width, int height) {
        diagnostic("drawOval(" + x + ", " + y + ", " + width + ", " + height + ")");
        doArc(x, y, width, height, 0, 360, false);
    }

    public void fillOval(int x, int y, int width, int height) {
        diagnostic("fillOval(" + x + ", " + y + ", " + width + ", " + height + ")");
        doArc(x, y, width, height, 0, 360, true);
    }

    protected void doArc(int x, int y, int width, int height, int startAngle, int arcAngle, boolean fill) {
        diagnostic("doArc(" + x + ", " + y + ", " + width + ", " + height + startAngle + ", " + arcAngle + ", " + fill + ")");
        y = transformY(y);
        gsave();
        int cx = x + width / 2;
        int cy = y - height / 2;
        translate(cx, cy);
        float yscale = (float)height / (float)width;
        scale(1.0D, yscale);
        if(fill)
            moveto(0, 0);
        os.println("0 setlinewidth");
        float endAngle = startAngle + arcAngle;
        os.print("0 0 ");
        os.print((double)(float)width / 2D);
        os.print(" ");
        os.print(startAngle);
        os.print(" ");
        os.print(endAngle);
        os.println(" arc");
        if(fill)
            os.println("closepath");
        stroke(fill);
        os.println("1 setlinewidth");
        grestore();
    }

    public void drawArc(int x, int y, int width, int height, int startAngle, int arcAngle) {
        diagnostic("drawArc(" + x + ", " + y + ", " + width + ", " + height + ", " + startAngle + ", " + arcAngle + ")");
        doArc(x, y, width, height, startAngle, arcAngle, false);
    }

    public void fillArc(int x, int y, int width, int height, int startAngle, int arcAngle) {
        diagnostic("fillArc(" + x + ", " + y + ", " + width + ", " + height + ", " + startAngle + ", " + arcAngle + ")");
        doArc(x, y, width, height, startAngle, arcAngle, true);
    }

    protected void doPoly(int xPoints[], int yPoints[], int nPoints, boolean fill, boolean close) {
        diagnostic("doPoly(" + xPoints.length + ", " + yPoints.length + ", " + nPoints + ", " + fill + ", " + close + ")");
        if(nPoints < 2)
            return;
        int newYPoints[] = new int[nPoints];
        for(int i = 0; i < nPoints; i++)
            newYPoints[i] = transformY(yPoints[i]);

        moveto(xPoints[0], newYPoints[0]);
        for(int i = 0; i < nPoints; i++)
            lineto(xPoints[i], newYPoints[i]);

        stroke(fill);
    }

    public void drawPolyline(int xPoints[], int yPoints[], int nPoints) {
        diagnostic("drawPolyline(" + xPoints.length + ", " + yPoints.length + ", " + nPoints + ")");
        doPoly(xPoints, yPoints, nPoints, false, false);
    }

    public void drawPolygon(int xPoints[], int yPoints[], int nPoints) {
        diagnostic("drawPolygon(" + xPoints.length + ", " + yPoints.length + ", " + nPoints + ")");
        doPoly(xPoints, yPoints, nPoints, false, true);
    }

    public void drawPolygon(Polygon p) {
        diagnostic("drawPolygon(" + p + ")");
        doPoly(p.xpoints, p.ypoints, p.npoints, false, true);
    }

    public void fillPolygon(int xPoints[], int yPoints[], int nPoints) {
        diagnostic("fillPolygon(" + xPoints.length + ", " + yPoints.length + ", " + nPoints + ")");
        doPoly(xPoints, yPoints, nPoints, true, true);
    }

    public void fillPolygon(Polygon p) {
        diagnostic("fillPolygon(" + p + ")");
        doPoly(p.xpoints, p.ypoints, p.npoints, true, true);
    }

    public void drawString(String str, int x, int y) {
        diagnostic("drawString(" + str + ", " + x + ", " + y + ")");
        y = transformY(y);
        moveto(x, y);
        os.print(" (");
        os.print(str);
        os.println(") show stroke");
    }

    public void drawString( AttributedCharacterIterator aci, int x,int y)
    {
    }

    public void drawChars(char data[], int offset, int length, int x, int y) {
        diagnostic("drawChars(" + data.length + ", " + offset + ", " + length + ", " + x + ", " + y + ")");
        drawString(new String(data, offset, length), x, y);
    }

    public void drawBytes(byte data[], int offset, int length, int x, int y) {
        diagnostic("drawBytes(" + data.length + ", " + offset + ", " + length + ", " + x + ", " + y + ")");
        drawString(new String(data, offset, length), x, y);
    }

    protected boolean doImage(Image img, int x, int y, int width, int height, int sx, int sy,
            int sw, int sh, ImageObserver observer, Color bgcolor) {
        diagnostic("doImage(" + img + ", " + x + ", " + y + ", " + width + ", " + height + ", " + sx + ", " + sy + ", " + sw + ", " + sh + ", " + observer + ", " + bgcolor + ")");
        int imgWidth = img.getWidth(observer);
        int imgHeight = img.getHeight(observer);
        y = transformY(y);
        int pix[] = new int[imgWidth * imgHeight];
        PixelGrabber pg = new PixelGrabber(img, 0, 0, imgWidth, imgHeight, pix, 0, imgWidth);
        boolean result = false;
        try {
            result = pg.grabPixels();
        }
        catch(InterruptedException _ex) { }
        finally {
            if(!result) {
                os.println("%warning: error on image grab");
                System.err.println("warning: error on image grab: " + pg.getStatus());
                return false;
            }
        }
        if(height < 1 || width < 1) {
            height = imgHeight;
            width = imgWidth;
        }
        int iLower = sy != 0 ? sx : 0;
        int iUpper = sh != 0 ? sy + sh : imgHeight;
        int jLower = sx != 0 ? sx : 0;
        int jUpper = sw != 0 ? sx + sw : imgWidth;
        int numYPixels = iUpper - iLower;
        int numXPixels = jUpper - jLower;
        gsave();
        os.println("% build a temporary dictionary");
        os.println("20 dict begin");
        emitColorImageProlog(numXPixels);
        os.println("% lower left corner");
        translate(x, y);
        os.println("% size of image");
        scale(width, height);
        os.print(numXPixels);
        os.print(" ");
        os.print(numYPixels);
        os.println(" 8");
        os.print("[");
        os.print(numXPixels);
        os.print(" 0 0 -");
        os.print(numYPixels);
        os.print(" 0 ");
        os.print(0);
        os.println("]");
        os.println("{currentfile pix readhexstring pop}");
        os.println("false 3 colorimage");
        os.println("");
        int sleepyet = 0;
        char sb[] = new char[73];
        int bg = bgcolor != null ? bgcolor.getRGB() : -1;
        for(int i = iLower; i < iUpper; i++) {
            int offset = 0;
            sleepyet++;
            for(int j = jLower; j < jUpper; j++) {
                int coord = i * imgWidth + j;
                int n = pix[coord];
                int alpha = n & 0xff000000;
                if(alpha == 0)
                    n = bg;
                sb[offset++] = hd[(n & 0xf00000) >> 20];
                sb[offset++] = hd[(n & 0xf0000) >> 16];
                sb[offset++] = hd[(n & 0xf000) >> 12];
                sb[offset++] = hd[(n & 0xf00) >> 8];
                sb[offset++] = hd[(n & 0xf0) >> 4];
                sb[offset++] = hd[n & 0xf];
                if(offset >= 72) {
                    os.write(sb, 0, offset);
                    os.println();
                    if(sleepyet > 5) {
                        try {
                            Thread.sleep(5L);
                        }
                        catch(InterruptedException _ex) { }
                        sleepyet = 0;
                    }
                    offset = 0;
                }
            }

            if(offset != 0) {
                os.write(sb, 0, offset);
                os.println();
            }
        }

        os.println();
        os.println("end");
        grestore();
        return true;
    }

    public boolean drawImage(Image img, int x, int y, ImageObserver observer) {
        diagnostic("drawImage(" + img + ", " + x + ", " + y + ", " + observer + ")");
        return doImage(img, x, y, 0, 0, 0, 0, 0, 0, observer, null);
    }

    public boolean drawImage(Image img, int x1, int y1, int x2, int y2, int x3, int y3,
            int x4, int y4, ImageObserver observer) {
        diagnostic("drawImage(" + img + ", " + x1 + ", " + y1 + ", " + x2 + ", " + y2 + ", " + x3 + ", " + y3 + ", " + x4 + ", " + y4 + ", " + observer + ")");
        return doImage(img, x1, y1, x2 - x1, y2 - y1, x3, y3, x4 - x3, y4 - y3, observer, null);
    }

    public boolean drawImage(Image img, int x1, int y1, int x2, int y2, int x3, int y3,
            int x4, int y4, Color c, ImageObserver observer) {
        diagnostic("drawImage(" + img + ", " + x1 + ", " + y1 + ", " + x2 + ", " + y2 + ", " + x3 + ", " + y3 + ", " + x4 + ", " + y4 + ", " + c + ", " + observer + ")");
        return doImage(img, x1, y1, x2 - x1, y2 - y1, x3, y3, x4 - x3, y4 - y3, observer, c);
    }

    public boolean drawImage(Image img, int x, int y, int width, int height, ImageObserver observer) {
        diagnostic("drawImage(" + img + "," + x + ", " + y + ", " + width + ", " + height + ", " + observer + ")");
        return doImage(img, x, y, width, height, 0, 0, 0, 0, observer, null);
    }

    public boolean drawImage(Image img, int x, int y, Color bgcolor, ImageObserver observer) {
        diagnostic("drawImage(" + img + ", " + x + ", " + y + ", " + bgcolor + ", " + observer + ")");
        return doImage(img, x, y, 0, 0, 0, 0, 0, 0, observer, bgcolor);
    }

    public boolean drawImage(Image img, int x, int y, int width, int height, Color bgcolor, ImageObserver observer) {
        diagnostic("drawImage(" + img + ", " + x + ", " + y + ", " + width + ", " + height + ", " + bgcolor + ", " + observer + ")");
        return doImage(img, x, y, width, height, 0, 0, 0, 0, observer, bgcolor);
    }

    public void startPage(int n) {
        os.println("%%Page: ? " + n);
    }

    public void endPage() {
        os.println("showpage");
    }

    public void dispose() {
        diagnostic("dispose() ");
        os.flush();
    }

    public void finalize() {
        diagnostic("finalize() ");
    }

    public String toString() {
        diagnostic("toString() ");
        return getClass().getName() + "[font=" + fontName + fontSize + ",color=" + getColor() + "]";
    }

    protected int transformY(int y) {
        return 846 - y;
    }

    protected void emitProlog() {
        os.println("%!PS-Adobe-2.0: EPSF-1.2 Created by PSGr Java PostScript Context");
        os.println("% PSGr is (C) 1996 Ernest Friedman-Hill and Sandia National Labs");
        os.println("% Right to unrestricted personal and commerical use is granted");
        os.println("% if this acknowledgement is given on product or packing materials");
        os.println("%%Creator: PSIPredView using PSGr PostScript Class.");
        os.println("%%Title: PSIPredView");
        os.println("% PSIPredView by L.J. McGuffin, K. Bryson and D.T. Jones (1999)");
        translate(30, -30);
        setFontNameSize(fontName, fontSize);
    }

    protected void emitColorImageProlog(int xdim) {
        os.println("% Color picture stuff, lifted from XV's PS files");
        os.println("% define string to hold a scanline's worth of data");
        os.print("/pix ");
        os.print(xdim * 3);
        os.println(" string def");
        os.println("% define space for color conversions");
        os.print("/grays ");
        os.print(xdim);
        os.println(" string def  % space for gray scale line");
        os.println("/npixls 0 def");
        os.println("/rgbindx 0 def");
        os.println("% define 'colorimage' if it isn't defined");
        os.println("%   ('colortogray' and 'mergeprocs' come from xwd2ps");
        os.println("%     via xgrab)");
        os.println("/colorimage where   % do we know about 'colorimage'?");
        os.println("{ pop }           % yes: pop off the 'dict' returned");
        os.println("{                 % no:  define one");
        os.println("/colortogray {  % define an RGB->I function");
        os.println("/rgbdata exch store    % call input 'rgbdata'");
        os.println("rgbdata length 3 idiv");
        os.println("/npixls exch store");
        os.println("/rgbindx 0 store");
        os.println("0 1 npixls 1 sub {");
        os.println("grays exch");
        os.println("rgbdata rgbindx       get 20 mul    % Red");
        os.println("rgbdata rgbindx 1 add get 32 mul    % Green");
        os.println("rgbdata rgbindx 2 add get 12 mul    % Blue");
        os.println("add add 64 idiv      % I = .5G + .31R + .18B");
        os.println("put");
        os.println("/rgbindx rgbindx 3 add store");
        os.println("} for");
        os.println("grays 0 npixls getinterval");
        os.println("} bind def");
        os.println("");
        os.println("% Utility procedure for colorimage operator.");
        os.println("% This procedure takes two procedures off the");
        os.println("% stack and merges them into a single procedure.");
        os.println("");
        os.println("/mergeprocs { % def");
        os.println("dup length");
        os.println("3 -1 roll");
        os.println("dup");
        os.println("length");
        os.println("dup");
        os.println("5 1 roll");
        os.println("3 -1 roll");
        os.println("add");
        os.println("array cvx");
        os.println("dup");
        os.println("3 -1 roll");
        os.println("0 exch");
        os.println("putinterval");
        os.println("dup");
        os.println("4 2 roll");
        os.println("putinterval");
        os.println("} bind def");
        os.println("");
        os.println("/colorimage { % def");
        os.println("pop pop     % remove 'false 3' operands");
        os.println("{colortogray} mergeprocs");
        os.println("image");
        os.println("} bind def");
        os.println("} ifelse          % end of 'false' case");
    }

    public void gsave() {
        os.println("gsave");
    }

    public void grestore() {
        os.println("grestore");
    }

    public void emitThis(String s) {
        os.println(s);
    }

    protected void diagnostic(String s) {
        os.print("% PSGR-");
        os.print(hashCode());
        os.print(": ");
        os.println(s);
    }

}
