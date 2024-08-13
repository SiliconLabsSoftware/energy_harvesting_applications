const ROOT = "../"
const PRODUCT_REF = "AEM13920"

const PORTAL_REV_TYPE = "n"
const PORTAL_VERSION = "4.0.0"
const PORTAL_GEN_DATE = "2024/12/04 08:44:24"
const PORTAL_PUBLISHED = false
const PORTAL_URL = "https://doc.e-peas.com/" + PRODUCT_REF.toLowerCase()
const PACK_PATH = "https://download.e-peas.com/packs/" + PRODUCT_REF.toLowerCase()

const sitemap = [
  {
    label: "Overview",
    key: "overview",
    url: ROOT+"overview/index.html",
    revision: 'n',
    version: "2.0.0",
    releaseNotes: ""
  },
  {
    label: "Software",
    key: "software",
    url: ROOT+"software/index.html",
    revision: 'n',
    version: "4.0.0",
    releaseNotes: ""
  },
  {
    label: "Downloads",
    key: "downloads",
    url: ROOT+"downloads/index.html",
    revision: "n",
    version: "1.0.0",
    releaseNotes: ""
  },
];

const sidenav_basewidth = 330;
if (document.cookie.indexOf("doxygen_width=") == -1) {
  var date = new Date();
  date.setTime(date.getTime()+(365*24*60*60*1000));
  expiration = date.toGMTString();
  document.cookie = "doxygen_width=" + sidenav_basewidth + "; SameSite=Lax; expires=" + expiration+"; path=/";
}
