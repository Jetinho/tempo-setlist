document.addEventListener('DOMContentLoaded', function () {
  fetch('data/setlist.json')
    .then((response) => response.json())
    .then((data) => {
      console.log(data);
      const form = document.getElementById('setlistForm');
      data.forEach((item, index) => {
        const details = document.createElement('details');
        const summary = document.createElement('summary');

        const itemTitle = item.title || 'Setlist Item';

        summary.textContent = `${item.position} - ${itemTitle}`;
        const fieldsRow = document.createElement('div');
        fieldsRow.className = 'fields-row';

        details.appendChild(summary);
        details.appendChild(fieldsRow);

        Object.keys(item).forEach((key) => {
          const box = document.createElement('div');
          box.style.display = 'flex';
          box.style.flexDirection = 'column';
          const label = document.createElement('label');
          label.textContent = key.charAt(0).toUpperCase() + key.slice(1);
          label.htmlFor = `${key}-${index}`;
          const input = document.createElement('input');
          input.id = `${key}-${index}`;
          if (['bpm', 'position', 'subdivision'].includes(key)) {
            input.type = 'number';
          }
          input.name = `setlist[${index}][${key}]`;
          input.value = item[key];
          box.appendChild(label);
          box.appendChild(input);
          fieldsRow.appendChild(box);
        });

        form.appendChild(details);
      });
      document.getElementById('setlist').appendChild(form);
    })
    .catch((error) => console.error('Error fetching setlist:', error));
});
