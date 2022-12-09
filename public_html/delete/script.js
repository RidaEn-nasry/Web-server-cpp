const input = document.getElementById('input0')
const d_btn = document.getElementById('btn_delete')
const c_btn = document.getElementById('btn_cancel')

	c_btn

	d_btn.addEventListener('click', () => {
		if (input.value === '' || input.value === 'index.html') {
			alert('Please enter a valid value')
 	 } 
 	 else {
			let url = `${window.location.origin}/`;
			url += input.value;
			console.log(url);
			console.log(input.value);
			fetch(url, {
				method: 'DELETE',
			})
			.then(res => {

				if ( res.status === 200 ) {
					alert('File deleted successfully')
					window.location.reload();
				}
				if ( res.status === 204 || res.status === 404) {
					alert('File not found')
					window.location.reload();
				}
				if (res.status === 400) {
					alert('Bad request')
					window.location.reload();
				}
				if (res.status === 500) {
					alert('Internal server error')
					window.location.reload();
				}
				if (res.status === 405) {
					alert('Method not allowed')
					window.location.reload();
				}
			})
		}
	})

	c_btn.addEventListener('click', () => {
		window.location.reload();
	})



