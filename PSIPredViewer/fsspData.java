// Processed by NMI's Java Code Viewer 4.8.2 1997-2000 B. Lemaire
// Website: http://njcv.htmlplanet.com	E-mail: info@njcv.htmlplanet.com
// Copy registered to Evaluation Copy
// Source File Name:   fsspData.java

import java.util.Vector;

public class fsspData {

    private String strAAsequence;
    private String strSecondaryStructure;
    private String strAccess;
    private String strZ_score;
    private Vector vConf;
    private int iz;
    private Vector vHstart;
    private Vector vHfin;
    private Vector vBstart;
    private Vector vBfin;
    private Vector vTstart;
    private Vector vTfin;
    private Vector vNstart;
    private Vector vNfin;
    private Vector vCysastart;
    private Vector vCysafin;
    private int h2;
    private int b2;
    private int t2;
    private int l2;
    private int i2;
    private int j2;
    public boolean datasetup;

    public fsspData(String AAsequence, String SecondaryStructure, String Access, String Z_score, Vector Conf, int z) {
        datasetup = false;
        strAAsequence = AAsequence;
        strSecondaryStructure = SecondaryStructure;
        strAccess = Access;
        strZ_score = Z_score;
        iz = z;
        vConf = Conf;
    }

    public void setVectors(Vector Hposstart, Vector Hposfinish, Vector Bposstart, Vector Bposfinish, Vector Tposstart, Vector Tposfinish, Vector Nposstart,
            Vector Nposfinish, Vector Cysastart, Vector Cysafinish, int h1, int b1, int t1, int l1) {
        vHstart = Hposstart;
        vHfin = Hposfinish;
        vBstart = Bposstart;
        vBfin = Bposfinish;
        vTstart = Tposstart;
        vTfin = Tposfinish;
        vNstart = Nposstart;
        vNfin = Nposfinish;
        vCysastart = Cysastart;
        vCysafin = Cysafinish;
        h2 = h1;
        b2 = b1;
        t2 = t1;
        l2 = l1;
    }

    public void setFontAtrr(int i1, int j1) {
        i1 = i2;
        j1 = j2;
    }

    public String getAAsequence() {
        return strAAsequence;
    }

    public void setAAsequence(String newAAsequence) {
        strAAsequence = newAAsequence;
    }

    public String getSecondaryStructure() {
        return strSecondaryStructure;
    }

    public void setSecStruct(String newSecStruct) {
        strSecondaryStructure = newSecStruct;
    }

    public String getAccess() {
        return strAccess;
    }

    public String getZ_score() {
        return strZ_score;
    }

    public Vector getConf() {
        return vConf;
    }

    public void setConf(Vector newConf) {
        vConf = newConf;
    }

    public int getNumberOfZ() {
        return iz;
    }

    public Vector getHstart() {
        return vHstart;
    }

    public void setHstart(Vector newHstart) {
        vHstart = newHstart;
    }

    public Vector getHfin() {
        return vHfin;
    }

    public void setHfin(Vector newHfin) {
        vHfin = newHfin;
    }

    public Vector getBstart() {
        return vBstart;
    }

    public void setBstart(Vector newBstart) {
        vBstart = newBstart;
    }

    public Vector getBfin() {
        return vBfin;
    }

    public void setBfin(Vector newBfin) {
        vBfin = newBfin;
    }

    public Vector getNstart() {
        return vNstart;
    }

    public void setNstart(Vector newNstart) {
        vNstart = newNstart;
    }

    public Vector getNfin() {
        return vNfin;
    }

    public void setNfin(Vector newNfin) {
        vNfin = newNfin;
    }

    public Vector getTstart() {
        return vTstart;
    }

    public void setTstart(Vector newTstart) {
        vTstart = newTstart;
    }

    public Vector getTfin() {
        return vTfin;
    }

    public void setTfin(Vector newTfin) {
        vTfin = newTfin;
    }

    public Vector getCysastart() {
        return vCysastart;
    }

    public Vector getCysafin() {
        return vCysafin;
    }

    public int getHNo() {
        return h2;
    }

    public int getBNo() {
        return b2;
    }

    public int getTNo() {
        return t2;
    }

    public int getl() {
        return l2;
    }

    public void setl(int newl) {
        l2 = newl;
    }

    public int geti() {
        return i2;
    }

    public void seti(int newi) {
        i2 = newi;
    }

    public int getj() {
        return j2;
    }

    public void setj(int newj) {
        j2 = newj;
    }
}
