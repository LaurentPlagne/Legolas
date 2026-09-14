// Automatically redirect requests ending with .md or .md/ to the corresponding directory URL
(function() {
  var pathname = window.location.pathname;
  if (pathname.endsWith('.md') || pathname.endsWith('.md/')) {
    var clean = pathname.replace(/\.md\/?$/, '/');
    window.location.replace(clean + window.location.search + window.location.hash);
  }
})();
