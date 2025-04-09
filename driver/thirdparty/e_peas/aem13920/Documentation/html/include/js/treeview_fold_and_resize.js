document.addEventListener("DOMContentLoaded", function () {
  let treeView = document.querySelector("#side-nav");
  let content = document.querySelector("#doc-content");

  let fold_icon = "&#x2B9C;"; // Unicode left arrow (⬅)
  let unfold_icon = "&#x2B9E;"; // Unicode right arrow (➡)

  if (!treeView || !content) return; // Ensure elements exist

  // Create the toggle button dynamically
  let toggleButton = document.createElement("button");
  toggleButton.id = "treeview-toggle";
  toggleButton.innerHTML = fold_icon;
  toggleButton.classList.add("toggle-button", "inside-sidebar");
  treeView.appendChild(toggleButton);

  let isCollapsed = false;
  let sidebarWidth = treeView.offsetWidth; // Store sidebar width
  let defaultWidth = sidebarWidth; // Store default width
  let minWidth = defaultWidth / 2; // Default width for desktop

  // Ensure button is always at the correct position
  function updateButtonPosition() {
      if (!isCollapsed) {
          toggleButton.innerHTML = fold_icon;
          toggleButton.classList.remove("outside-sidebar");
          toggleButton.classList.add("inside-sidebar");
          toggleButton.style.left = "auto"; // Reset left when inside
          toggleButton.style.right = "10px"; // Always near right edge
          treeView.appendChild(toggleButton);
          treeView.style.pointerEvents = "auto"; // Re-enable interaction
      } else {
          toggleButton.innerHTML = unfold_icon;
          toggleButton.classList.remove("inside-sidebar");
          toggleButton.classList.add("outside-sidebar");
          toggleButton.style.left = "0px"; // Stick to left screen edge
          toggleButton.style.right = "auto"; // Reset right when outside
          document.body.appendChild(toggleButton);
          treeView.style.pointerEvents = "none"; // Disable interaction with collapsed sidebar
      }
  }

  // Toggle function
  toggleButton.addEventListener("click", function () {
      if (isCollapsed) {
          // Expand sidebar
          treeView.style.width = sidebarWidth + "px";
          content.style.marginLeft = (sidebarWidth + 10) + "px";
      } else {
          // Collapse sidebar
          sidebarWidth = treeView.offsetWidth; // Save width before hiding
          treeView.style.width = "0px";
          content.style.marginLeft = "10px";
      }
      isCollapsed = !isCollapsed;
      updateButtonPosition();
  });

  // **Double-clicking on the sidebar edge resizes it to fit-content**
  treeView.addEventListener("dblclick", function (event) {
    if (event.offsetX > treeView.offsetWidth - 10) { // Click near the edge
        treeView.style.width = defaultWidth + "px"; // Resize to fit content
        sidebarWidth = treeView.offsetWidth; // Update stored width
        content.style.marginLeft = (sidebarWidth + 10) + "px";
        isCollapsed = false; // Ensure state is updated
        updateButtonPosition();
    }
  });

  // Ensure correct button placement on window resize
  window.addEventListener("resize", function () {
      if (window.innerWidth <= 768) {
          treeView.style.resize = "none"; // Disable resizing
          treeView.style.pointerEvents = "none"; // Disable interaction with collapsed sidebar
          treeView.style.width = "fit-content"; // Default width for mobile
          minWidth = treeView.offsetWidth; // Store new width
          content.style.marginLeft = "0px";
          isCollapsed = false;
      } else if (!isCollapsed) {
          treeView.style.pointerEvents = "auto"; // Re-enable interaction
          minWidth = defaultWidth; // Reset to default width
          treeView.style.width = sidebarWidth + "px"; // Use stored width
          content.style.marginLeft = (sidebarWidth + 10) + "px";
      }
      updateButtonPosition();
  });

  // Keep button in correct place on sidebar resize
  new ResizeObserver(() => {
    if (treeView.offsetWidth < minWidth) {
      treeView.style.pointerEvents = "none"; // Disable interaction with collapsed sidebar
      isCollapsed = true;
      treeView.style.width = "0px";
      content.style.marginLeft = "10px";
      document.dispatchEvent(new MouseEvent("mouseup"));
      updateButtonPosition();
    } else if (treeView.offsetWidth >= minWidth && isCollapsed) {
      isCollapsed = false;
      treeView.style.width = sidebarWidth + "px";
      content.style.marginLeft = (sidebarWidth + 10) + "px";
      updateButtonPosition();
    }
  }).observe(treeView);
});
