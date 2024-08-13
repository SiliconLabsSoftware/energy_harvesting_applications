// Get current section
var CURRENT_SECTION;
var url = location.pathname.substring(0, location.pathname.lastIndexOf("/"));
for (var i in sitemap) {
	if (url.match(sitemap[i].key)) {
		CURRENT_SECTION = sitemap[i];
		break;
	}
}