var menudata = {children:[]};

for (let p of sitemap) {
	if (["overview","datasheet","software"].includes(p.key)) {
		menudata.children.push({text:p.label, url:p.url});
	}
}

if ((typeof PORTAL_URL !== 'undefined')) {
	menudata.children.push({text:"Web portal", url:PORTAL_URL})
}