// Processed by NMI's Java Code Viewer 4.8.2 1997-2000 B. Lemaire
// Website: http://njcv.htmlplanet.com	E-mail: info@njcv.htmlplanet.com
// Copy registered to Evaluation Copy
// Source File Name:   fsspCanvas2.java

import java.awt.Color;
import java.awt.Dimension;
import java.io.IOException;


public class fsspCanvas2 {

    protected static final int PAGEHEIGHT = 846;
    protected static final int PAGEWIDTH = 594;
    protected static final int XOFFSET = 72;
    protected static final int YOFFSET = 72;
    private int strwidth;
    private fsspData data;
    private boolean initialized;
    private float charWidth;
    private int charWidth2;
    private int charWidth4;
    private int charHeight;
    private int charHeight2;
    private int charHeight4;
    private int seqLength;
    private Dimension pagesize;
    private int numberBlocks;
    private int blockHeightPixels;
    private int blocksPerPage;
    private int numberPages;

    public fsspCanvas2(String inputFilename) throws IOException {
        strwidth = 40;
        initialized = false;
        data = fsspProgFile.readSecStruct(inputFilename);
        data.datasetup = true;
    }

    public boolean print(PSGraphics g, int pageNum) {

		g.setFontNameSize("Monospaced", 12);

		/*Graphics2D g2 = (Graphics2D) g;
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,   // Anti-alias!
			    RenderingHints.VALUE_ANTIALIAS_ON);
		*/
		if(!initialized) {
            charWidth = g.getFontWidth();
            charWidth2 = (int)(charWidth / 2.0F);
            charWidth4 = (int)(charWidth / 4F);
            charHeight = g.getFontHeight();
            charHeight2 = charHeight / 2;
            charHeight4 = charHeight / 4;
            seqLength = data.getl();
            pagesize = new Dimension(450, 702);
            numberBlocks = (seqLength - 1) / strwidth + 1;
            blockHeightPixels = 8 * charHeight;
            blocksPerPage = pagesize.height / blockHeightPixels;
            numberPages = (numberBlocks - 1) / blocksPerPage + 1;
            int blocksOnLastPage = numberBlocks - (numberPages - 1) * blocksPerPage;
            int spaceLeft = pagesize.height - blocksOnLastPage * blockHeightPixels;
            if(spaceLeft < 9 * charHeight)
                numberPages++;
            initialized = true;
        }
        int substrwidth = (int)((float)strwidth * charWidth);
        g.translate((pagesize.width - substrwidth) / 2 + 72, -72);
        if(pageNum < numberPages) {
            g.setFontNameSize("Monospaced", 12);
            g.startPage(pageNum + 1);
            g.setColor(Color.black);
            int blockNum = 0;
            int resStart = pageNum * blocksPerPage * strwidth;
            for(blockNum = 0; blockNum < blocksPerPage && resStart < seqLength; resStart += strwidth) {
                int resEnd = resStart + strwidth;
                if(resEnd > seqLength) {
                    resEnd = seqLength;
                    substrwidth = (int)((float)(resEnd - resStart) * charWidth);
                }
                String resString = data.getAAsequence().substring(resStart, resEnd);
                String ssString = data.getSecondaryStructure().substring(resStart, resEnd);
                int layer1 = blockNum * blockHeightPixels + charHeight;
                int layer2 = layer1 + charHeight;
                int layer3 = layer2 + charHeight;
                int layer25 = layer2 + charHeight2;
                int layer4 = layer3 + charHeight;
                int layer5 = layer4 + charHeight;
                int layer6 = layer5 + charHeight;
                int layer7 = layer6 + charHeight;
                int layer8 = layer7 + charHeight + charHeight2;
                drawConfidenceBars(g, resStart, resEnd, layer2);
                g.setColor(Color.black);
                g.drawLine(-charWidth2, layer2, -charWidth4, layer2);
                g.drawLine(-charWidth2, layer3, -charWidth4, layer3);
                g.drawLine(-charWidth2, layer25, -charWidth4, layer25);
                g.drawLine(-charWidth4, layer2, -charWidth4, layer3);
                g.drawString("Conf:", (int)(-6F * charWidth), layer3);
                g.drawLine(substrwidth + charWidth4, layer2, substrwidth + charWidth4, layer3);
                g.drawLine(substrwidth + charWidth2, layer2, substrwidth + charWidth4, layer2);
                g.drawLine(substrwidth + charWidth2, layer3, substrwidth + charWidth4, layer3);
                g.drawLine(substrwidth + charWidth2, layer25, substrwidth + charWidth4, layer25);
                g.drawLine(substrwidth + charWidth4, layer2, substrwidth + charWidth4, layer3);
                drawSecondaryRegion(g, resStart, resEnd, layer4);
                g.setColor(Color.black);
                g.drawString("Pred:", (int)(-6F * charWidth), layer5 - charHeight2);
                g.drawString(ssString, 0, layer6);
                g.drawString("Pred:", (int)(-6F * charWidth), layer6);
                g.drawString(resString, 0, layer7);
                g.drawString("AA:", (int)(-4F * charWidth), layer7);
                for(int k = 10; k <= resEnd - resStart; k += 10) {
                    g.drawLine((int)((float)k * charWidth - (float)charWidth2), layer7 + charHeight4, (int)((float)k * charWidth - (float)charWidth2), layer7 + charHeight2);
                    g.drawString(String.valueOf(k + resStart), (int)((float)(k - 1) * charWidth), layer8);
                }

                blockNum++;
            }

            if(pageNum == numberPages - 1)
                drawLegend(g, 0, blockNum * blockHeightPixels + 3 * charHeight);
            g.endPage();
            return true;
        } else {
            return false;
        }
    }

    public void drawConfidenceBars(PSGraphics g, int start, int end, int y) {
        int q = 0;
        for(q = 0; q < data.getConf().size(); q++) {
            float qf = ((Float)data.getConf().elementAt(q)).floatValue();
            int q1 = (int)(qf * (float)charHeight);
            if(q >= start && q <= end - 1) {
                int xValues[] = {
                    (int)((float)(q - start) * charWidth + (float)charWidth4), (int)((float)((q - start) + 1) * charWidth - (float)charWidth4), (int)((float)((q - start) + 1) * charWidth - (float)charWidth4), (int)((float)(q - start) * charWidth + (float)charWidth4)
                };
                int yValues[] = {
                    y + charHeight, y + charHeight, (y + charHeight) - q1, (y + charHeight) - q1
                };
				System.out.println();
				float scaled_colour = 255 - (((float)q1/(float)charHeight) * 255);
				Color myBlue = new Color ((int)scaled_colour,(int)scaled_colour, 255);
                g.setColor(myBlue);
                g.fillPolygon(xValues, yValues, 4);
                g.setColor(Color.black);
                g.drawPolygon(xValues, yValues, 4);

            }
        }

    }

    public void drawSecondaryRegion(PSGraphics g, int start, int end, int y) {
        int width = end - start;
        int b = 0;
        int h = 0;
        int t = 0;
        int n = 0;
		Color myPurple = new Color (255, 136, 255);
		Color myYellow = new Color (255, 255, 136);
		Color myRed = new Color (255, 136, 136);
		Color myPink = new Color (255, 200, 200);

        for(h = 0; h < data.getHstart().size(); h++) {
            int x1 = ((Integer)data.getHstart().elementAt(h)).intValue();
            int x2 = ((Integer)data.getHfin().elementAt(h)).intValue();
            if(x1 >= start && x1 <= end - 1 && x2 >= start && x2 <= end - 1) {
                x1 -= start;
                x2 -= start;
                g.setColor(myPurple);
                g.fillRect((int)((float)x1 * charWidth), y, (int)((float)((x2 + 1) - x1) * charWidth), charHeight);
                g.setColor(myPink);
                g.fillOval((int)((float)x1 * charWidth - (float)charWidth2), y, (int)charWidth, charHeight);
                g.setColor(Color.black);
                g.drawOval((int)((float)x1 * charWidth - (float)charWidth2), y, (int)charWidth, charHeight);
            } else
            if(x1 >= start && x1 <= end - 1 && x2 > start && x2 > end - 1) {
                x1 -= start;
                x2 = width - 1;
                g.setColor(myPurple);
                g.fillRect((int)((float)x1 * charWidth), y, (int)((float)((x2 + 1) - x1) * charWidth), charHeight);
                g.setColor(myPink);
                g.fillOval((int)((float)x1 * charWidth - (float)charWidth2), y, (int)charWidth, charHeight);
                g.setColor(Color.black);
                g.drawOval((int)((float)x1 * charWidth - (float)charWidth2), y, (int)charWidth, charHeight);
            } else
            if(x1 < start && x1 < end - 1 && x2 > start && x2 > end - 1) {
                x1 = 0;
                x2 = width - 1;
                g.setColor(myPurple);
                g.fillRect((int)((float)x1 * charWidth), y, (int)((float)((x2 + 1) - x1) * charWidth), charHeight);
                g.setColor(Color.black);
                g.drawLine((int)((float)x1 * charWidth), y, (int)((float)(x2 + 1) * charWidth), y);
                g.drawLine((int)((float)x1 * charWidth), y + charHeight, (int)((float)(x2 + 1) * charWidth), y + charHeight);
            } else
            if(x1 < start && x1 < end - 1 && x2 >= start && x2 <= end - 1) {
                x1 = 0;
                x2 -= start;
                g.setColor(myPurple);
                g.fillRect((int)((float)x1 * charWidth), y, (int)((float)((x2 + 1) - x1) * charWidth), charHeight);
            }
        }

        for(n = 0; n < data.getNstart().size(); n++) {
            int v1 = ((Integer)data.getNstart().elementAt(n)).intValue();
            int v2 = ((Integer)data.getNfin().elementAt(n)).intValue();
            if(v1 >= start && v1 <= end - 1 && v2 >= start && v2 <= end - 1) {
                v1 -= start;
                v2 -= start;
                g.setColor(Color.black);
                g.drawLine((int)((float)v1 * charWidth), y + charHeight2, (int)((float)(v2 + 1) * charWidth), y + charHeight2);
            } else
            if(v1 >= start && v1 <= end - 1 && v2 > start && v2 > end - 1) {
                v1 -= start;
                v2 = width - 1;
                g.setColor(Color.black);
                g.drawLine((int)((float)v1 * charWidth), y + charHeight2, (int)((float)(v2 + 1) * charWidth), y + charHeight2);
            } else
            if(v1 < start && v1 < end - 1 && v2 >= start && v2 < end - 1) {
                v1 = 0;
                v2 -= start;
                g.setColor(Color.black);
                g.drawLine((int)((float)v1 * charWidth), y + charHeight2, (int)((float)(v2 + 1) * charWidth), y + charHeight2);
            } else
            if(v1 < start && v1 < end - 1 && v2 > start && v2 > end - 1) {
                v1 = 0;
                v2 = width - 1;
                g.setColor(Color.black);
                g.drawLine((int)((float)v1 * charWidth), y + charHeight2, (int)((float)(v2 + 1) * charWidth), y + charHeight2);
            }
        }

        for(t = 0; t < data.getTstart().size(); t++) {
            int u1 = ((Integer)data.getTstart().elementAt(t)).intValue();
            int u2 = ((Integer)data.getTfin().elementAt(t)).intValue();
            g.setColor(Color.white);
            g.drawLine((int)((float)u1 * charWidth), y, (int)((float)u1 * charWidth), y);
            if(u1 >= start && u1 <= end - 1 && u2 >= start && u2 <= end - 1) {
                u1 -= start;
                u2 -= start;
                g.setColor(Color.orange);
                g.drawArc((int)((float)u1 * charWidth), y, (int)((float)((u2 + 1) - u1) * charWidth), charHeight, 0, 180);
            } else
            if(u1 >= start && u1 <= end - 1 && u2 > start && u2 > end - 1) {
                u1 -= start;
                u2 = width - 1;
                g.setColor(Color.orange);
                g.drawArc((int)((float)u1 * charWidth), y, (int)charWidth, charHeight, 90, 90);
                g.drawLine((int)((float)(u1 + 1) * charWidth - (float)charWidth2), y, (int)((float)(u2 + 1) * charWidth), y);
            } else
            if(u1 < start && u1 < end - 1 && u2 >= start && u2 < end - 1) {
                u1 = 0;
                u2 -= start;
                g.setColor(Color.orange);
                g.drawArc((int)((float)u2 * charWidth), y, (int)charWidth, charHeight, 0, 90);
                g.drawLine((int)((float)u1 * charWidth - (float)charWidth2), y, (int)((float)u2 * charWidth + (float)charWidth2), y);
            } else
            if(u1 < start && u1 < end - 1 && u2 > start && u2 > end - 1) {
                u1 = 0;
                u2 = width - 1;
                g.setColor(Color.orange);
                g.drawLine((int)((float)u1 * charWidth), y - charHeight2, (int)((float)u2 * charWidth), y - charHeight2);
            }
        }

        for(b = 0; b < data.getBstart().size(); b++) {
            int w1 = ((Integer)data.getBstart().elementAt(b)).intValue();
            int w2 = ((Integer)data.getBfin().elementAt(b)).intValue();
            if(w1 >= start && w1 <= end - 1 && w2 >= start && w2 <= end - 1) {
                w1 -= start;
                w2 -= start;
                int xValues[] = {
                    (int)((float)w1 * charWidth), (int)((float)w2 * charWidth), (int)((float)w2 * charWidth), (int)((float)(w2 + 1) * charWidth), (int)((float)w2 * charWidth), (int)((float)w2 * charWidth), (int)((float)w1 * charWidth), (int)((float)w1 * charWidth)
                };
                int yValues[] = {
                    y + charHeight4, y + charHeight4, y, y + charHeight2, y + charHeight, y + 3 * charHeight4, y + 3 * charHeight4, y + charHeight4
                };
                g.setColor(myYellow);
                g.fillPolygon(xValues, yValues, 8);
                g.setColor(myRed);
                g.drawPolygon(xValues, yValues, 8);
            } else
            if(w1 >= start && w1 <= end - 1 && w2 > start && w2 > end - 1) {
                w1 -= start;
                w2 = width - 1;
                g.setColor(myYellow);
                g.fillRect((int)((float)w1 * charWidth), y + charHeight4, (int)((float)((w2 + 1) - w1) * charWidth), charHeight2);
                g.setColor(myRed);
                g.drawLine((int)((float)w1 * charWidth), y + charHeight4, (int)((float)(w2 + 1) * charWidth - 1.0F), y + charHeight4);
                g.drawLine((int)((float)w1 * charWidth), y + 3 * charHeight4, (int)((float)(w2 + 1) * charWidth - 1.0F), y + 3 * charHeight4);
                g.drawLine((int)((float)w1 * charWidth), y + charHeight4, (int)((float)w1 * charWidth), y + 3 * charHeight4);
            } else
            if(w1 < start && w1 < end - 1 && w2 >= start && w2 <= end - 1) {
                w1 = 0;
                w2 -= start;
                int xValues[] = {
                    (int)((float)w1 * charWidth), (int)((float)w2 * charWidth), (int)((float)w2 * charWidth), (int)((float)(w2 + 1) * charWidth), (int)((float)w2 * charWidth), (int)((float)w2 * charWidth), (int)((float)w1 * charWidth), (int)((float)w1 * charWidth)
                };
                int yValues[] = {
                    y + charHeight4, y + charHeight4, y, y + charHeight2, y + charHeight, y + 3 * charHeight4, y + 3 * charHeight4, y + charHeight4
                };
                g.setColor(myYellow);
                g.fillPolygon(xValues, yValues, 8);
                g.setColor(myRed);
                g.drawLine((int)((float)w1 * charWidth), y + charHeight4, (int)((float)w2 * charWidth), y + charHeight4);
                g.drawLine((int)((float)w2 * charWidth), y + charHeight4, (int)((float)w2 * charWidth), y);
                g.drawLine((int)((float)w2 * charWidth), y, (int)((float)(w2 + 1) * charWidth), y + charHeight2);
                g.drawLine((int)((float)w2 * charWidth), y + charHeight, (int)((float)(w2 + 1) * charWidth), y + charHeight2);
                g.drawLine((int)((float)w2 * charWidth), y + 3 * charHeight4, (int)((float)w2 * charWidth), y + charHeight);
                g.drawLine((int)((float)w2 * charWidth), y + 3 * charHeight4, (int)((float)w1 * charWidth), y + 3 * charHeight4);
            } else
            if(w1 < start && w1 < end - 1 && w2 > start && w2 > end - 1) {
                w1 = 0;
                w2 = width - 1;
                g.setColor(myYellow);
                g.fillRect((int)((float)w1 * charWidth), y + charHeight4, (int)((float)((w2 + 1) - w1) * charWidth), charHeight2);
                g.setColor(myRed);
                g.drawLine((int)((float)w1 * charWidth), y + charHeight4, (int)((float)(w2 + 1) * charWidth), y + charHeight4);
                g.drawLine((int)((float)w1 * charWidth), y + 3 * charHeight4, (int)((float)(w2 + 1) * charWidth), y + 3 * charHeight4);
            }
        }

        for(h = 0; h < data.getHstart().size(); h++) {
            int x1 = ((Integer)data.getHstart().elementAt(h)).intValue();
            int x2 = ((Integer)data.getHfin().elementAt(h)).intValue();
            if(x1 >= start && x1 <= end - 1 && x2 >= start && x2 <= end - 1) {
                x1 -= start;
                x2 -= start;
                g.setColor(myPurple);
                g.fillOval((int)((float)x2 * charWidth + (float)charWidth2), y, (int)charWidth, charHeight);
                g.drawOval((int)((float)x2 * charWidth + (float)charWidth2), y, (int)charWidth, charHeight);
                g.setColor(Color.black);
                g.drawLine((int)((float)x1 * charWidth), y, (int)((float)(x2 + 1) * charWidth), y);
                g.drawArc((int)((float)x1 * charWidth - (float)charWidth2), y, (int)charWidth, charHeight, 270, 180);
                g.drawLine((int)((float)x1 * charWidth), y + charHeight, (int)((float)(x2 + 1) * charWidth), y + charHeight);
                g.drawArc((int)((float)x2 * charWidth + (float)charWidth2), y, (int)charWidth, charHeight, 270, 180);
            } else
            if(x1 >= start && x1 <= end - 1 && x2 > start && x2 > end - 1) {
                x1 -= start;
                x2 = width - 1;
                g.setColor(Color.black);
                g.drawLine((int)((float)x1 * charWidth), y, (int)((float)(x2 + 1) * charWidth), y);
                g.drawArc((int)((float)x1 * charWidth - (float)charWidth2), y, (int)charWidth, charHeight, 270, 180);
                g.drawLine((int)((float)x1 * charWidth), y + charHeight, (int)((float)(x2 + 1) * charWidth), y + charHeight);
            } else
            if(x1 < start && x1 < end - 1 && x2 > start && x2 > end - 1) {
                x1 = 0;
                x2 = width - 1;
                g.setColor(myPurple);
                g.fillRect((int)((float)x1 * charWidth), y, (int)((float)((x2 + 1) - x1) * charWidth), charHeight);
                g.setColor(Color.black);
                g.drawLine((int)((float)x1 * charWidth), y, (int)((float)(x2 + 1) * charWidth), y);
                g.drawLine((int)((float)x1 * charWidth), y + charHeight, (int)((float)(x2 + 1) * charWidth), y + charHeight);
            } else
            if(x1 < start && x1 < end - 1 && x2 >= start && x2 <= end - 1) {
                x1 = 0;
                x2 -= start;
                g.setColor(myPurple);
                g.fillOval((int)((float)x2 * charWidth + (float)charWidth2), y, (int)charWidth, charHeight);
                g.setColor(Color.black);
                g.drawLine((int)((float)x1 * charWidth), y, (int)((float)(x2 + 1) * charWidth), y);
                g.drawLine((int)((float)x1 * charWidth), y + charHeight, (int)((float)(x2 + 1) * charWidth), y + charHeight);
                g.drawArc((int)((float)x2 * charWidth + (float)charWidth2), y, (int)charWidth, charHeight, 270, 180);
            }
        }

    }

    public void drawLegend(PSGraphics g, int x, int y) {

		Color myPurple = new Color (255, 136, 255);
    	Color myYellow = new Color (255, 255, 136);
    	Color myRed = new Color (255, 136, 136);
    	Color myPink = new Color (255, 200, 200);

		g.setFontNameSize("Monospaced", 10);
        g.setColor(Color.black);
        g.drawString("Legend:", -30, y - charHeight);
        g.drawRect(-40, y - 2 * charHeight, (int)(53F * charWidth), 7 * charHeight + charHeight2);

		//Draw helix example
		g.setColor(myPurple);
        g.fillRect(-30, y, (int)(5F * charWidth), charHeight);
        g.setColor(myPink);
        g.fillOval(-charWidth2-30, y, (int)charWidth, charHeight);
        g.setColor(myPurple);
        g.fillOval((int)(5F * charWidth - (float)charWidth2)-30, y, (int)charWidth, charHeight);
        g.setColor(Color.black);
        g.drawLine(-30, y, (int)(5F * charWidth)-30, y);
        g.drawLine(-30, y + charHeight, (int)(5F * charWidth)-30, y + charHeight);
        g.drawArc((int)(5F * charWidth - (float)charWidth2)-30, y, (int)charWidth, charHeight, 270, 180);
        g.drawOval(-charWidth2-30, y, (int)charWidth, charHeight);
        g.setColor(Color.black);
        g.drawString(" = helix", (int)(6F * charWidth)-30, y + charHeight);

		//draw strand example
		int xValues[] = {
            -30, (int)(4F * charWidth)-30, (int)(4F * charWidth)-30, (int)(5F * charWidth)-30, (int)(4F * charWidth)-30, (int)(4F * charWidth)-30, 0-30, 0-30
        };
        int yValues[] = {
            y + charHeight4 + 2 * charHeight, y + charHeight4 + 2 * charHeight, y + 2 * charHeight, y + charHeight2 + 2 * charHeight, y + 3 * charHeight, y + 3 * charHeight4 + 2 * charHeight, y + 3 * charHeight4 + 2 * charHeight, y + charHeight4 + 2 * charHeight
        };
        g.setColor(myYellow);
        g.fillPolygon(xValues, yValues, 8);
        g.setColor(myRed);
        g.drawPolygon(xValues, yValues, 8);
        g.setColor(Color.black);
        g.drawString(" = strand ", (int)(6F * charWidth)-30, y + 3 * charHeight);

		//draw coil
		g.setColor(Color.black);
        g.drawLine(-30, y + charHeight2 + 4 * charHeight, (int)(5F * charWidth)-30, y + charHeight2 + 4 * charHeight);
        g.drawString(" = coil ", (int)(6F * charWidth)-30, y + 5 * charHeight);


		for(int column = 0; column < 5; column++) {
            int position = (int)((float)(22 + column) * charWidth + (float)charWidth4) -30;
            int x1Values[] = {
                position, position + charWidth2, position + charWidth2, position
            };
            int y1Values[] = {
                y + charHeight, y + charHeight, y + ((8 - 2 * column) * charHeight) / 9, y + ((8 - 2 * column) * charHeight) / 9
            };
			float scaled_colour = 255-((float)63.75 * (float)column);
			//System.out.println("LEGEND VALUES: " + position + " " + column + " " + charHeight + " " + y);
			Color myBlue = new Color ((int)scaled_colour,(int)scaled_colour, 255);

            g.setColor(myBlue);
            g.fillPolygon(x1Values, y1Values, 4);
            g.setColor(Color.black);
            g.drawPolygon(x1Values, y1Values, 4);
        }

        g.setColor(Color.black);
        g.drawLine((int)(22F * charWidth - (float)charWidth4)-30, y, (int)(22F * charWidth - (float)charWidth4)-30, y + charHeight);
        g.drawLine((int)(22F * charWidth - (float)charWidth2)-30, y, (int)(22F * charWidth - (float)charWidth4)-30, y);
        g.drawLine((int)(22F * charWidth - (float)charWidth2)-30, y + charHeight, (int)(22F * charWidth - (float)charWidth4)-30, y + charHeight);
        g.drawLine((int)(22F * charWidth - (float)charWidth2)-30, y + charHeight2, (int)(22F * charWidth - (float)charWidth4)-30, y + charHeight2);
        g.drawLine((int)(22F * charWidth - (float)charWidth4)-30, y, (int)(22F * charWidth - (float)charWidth4)-30, y + charHeight);
        g.drawLine((int)(27F * charWidth + (float)charWidth4)-30, y, (int)(27F * charWidth + (float)charWidth4)-30, y + charHeight);
        g.drawLine((int)(27F * charWidth + (float)charWidth2)-30, y, (int)(27F * charWidth + (float)charWidth4)-30, y);
        g.drawLine((int)(27F * charWidth + (float)charWidth2)-30, y + charHeight, (int)(27F * charWidth + (float)charWidth4)-30, y + charHeight);
        g.drawLine((int)(27F * charWidth + (float)charWidth2)-30, y + charHeight2, (int)(27F * charWidth + (float)charWidth4)-30, y + charHeight2);
        g.drawLine((int)(27F * charWidth + (float)charWidth4)-30, y, (int)(27F * charWidth + (float)charWidth4)-30, y + charHeight);
        g.drawString("Conf: ", (int)(16F * charWidth)-30, y + charHeight);
        g.drawString(" = confidence of prediction", (int)(28F * charWidth)-30, y + charHeight);
        g.drawString("-", (int)(22F * charWidth)-30, y + 2 * charHeight);
        g.drawString("+", (int)(26F * charWidth)-30, y + 2 * charHeight);
        g.drawString("Pred: predicted secondary structure", (int)(-1F + 16F * charWidth)-30, y + 3 * charHeight);
        g.drawString("AA: target sequence", (int)(1.0F + 16F * charWidth)-30, y + 5 * charHeight);
        g.setFontNameSize("Monospaced", 12);
    }
}
