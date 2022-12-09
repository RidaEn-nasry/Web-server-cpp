const upload_file = document.getElementById('choose_file');


upload_file.addEventListener('change', function(event) {
	event.preventDefault();
	const file = this.files[0];
	if (!file.type.match('image.*')){
		alert('Please select an image file');
		return;
	}
	var data = new FormData();
	data.append('file', file);
	const url = `${window.location.origin}/cgi-bin/img_text.py`;
	fetch(url, {
		method: 'POST',
		body: data
	})
	.then(response => response.text())
	.then(html => {
		window.document.body.parentNode.innerHTML = html;
	})
	.catch(error => console.error(error));
});
