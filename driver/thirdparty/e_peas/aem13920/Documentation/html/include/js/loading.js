window.addEventListener('load', () => {
	
	// Use setTimeout to hide loading animation when page is fully loaded
	setTimeout(function(){
		
		// Hide loading div
		const loadingdiv = document.querySelector('#loading');
		loadingdiv.style.display = "none";
		
	});
});
