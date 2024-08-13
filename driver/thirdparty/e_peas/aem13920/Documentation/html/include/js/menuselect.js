/* 
 * Set class ".selected" to the menu item which is currently active.
 */
window.addEventListener('load', () => {
	// Use setTimeout to make sure it is executed after everything has rendered on the page
	setTimeout(function(){

		// Get all menu elements
		const $menuLinks = Array.prototype.slice.call(document.querySelectorAll('#main-menu > li > a' ), 0);
		// Set current menu item selected
		$menuLinks.every( el => {
			if (el.innerHTML == CURRENT_SECTION.label){
				el.classList.add('selected');
				return false;
			}
			return true;
		})
	});
});
