document.addEventListener('DOMContentLoaded', () => {
  fetch('data/setlist.json')
    .then((response) => response.json())
    .then((data) => {
      console.log(data);
      const form = document.getElementById('setlistForm');
      data.forEach((item, index) => {
        const details = createDetailsElement(item, index);
        form.appendChild(details);
      });
      document.getElementById('setlist').appendChild(form);
    })
    .catch((error) => console.error('Error fetching setlist:', error));
});

function createDetailsElement(item, index) {
  const details = document.createElement('details');
  const summary = document.createElement('summary');
  const itemTitle = item.title || 'Setlist Item';

  summary.textContent = `${item.position} - ${itemTitle}`;
  details.appendChild(summary);

  const fieldsRow = createFieldsRow(item, index);
  details.appendChild(fieldsRow);

  details.classList.add('draggable');
  details.draggable = true;

  addDragAndDropHandlers(details, index);

  return details;
}

function createFieldsRow(item, index) {
  const fieldsRow = document.createElement('div');
  fieldsRow.className = 'fields-row';

  Object.keys(item).forEach((key) => {
    const box = document.createElement('div');
    box.style.display = 'flex';
    box.style.flexDirection = 'column';

    const label = document.createElement('label');
    label.textContent = key.charAt(0).toUpperCase() + key.slice(1);
    label.htmlFor = `${key}-${index}`;

    const input = document.createElement('input');
    input.id = `${key}-${index}`;
    input.name = `setlist[${index}][${key}]`;
    input.value = item[key];

    if (['bpm', 'position', 'subdivision'].includes(key)) {
      input.type = 'number';
    }
    if (key === 'position') {
      input.readOnly = true;
    }

    box.appendChild(label);
    box.appendChild(input);
    fieldsRow.appendChild(box);
  });

  return fieldsRow;
}

function addDragAndDropHandlers(details, index) {
  details.addEventListener('dragstart', (e) => {
    e.dataTransfer.setData('text/plain', index);
    details.classList.add('dragging');
  });

  details.addEventListener('dragend', () => {
    details.classList.remove('dragging');
    updatePositions();
    document.querySelectorAll('summary').forEach((summary) => {
      summary.classList.remove('dropzone');
    });
  });

  details.addEventListener('dragover', (e) => {
    e.preventDefault();
    const draggingElement = document.querySelector('.dragging');
    const currentElement = e.target.closest('details');
    if (currentElement && currentElement !== draggingElement) {
      const bounding = currentElement.getBoundingClientRect();
      const offset = bounding.y + bounding.height / 2;
      if (e.clientY - offset > 0) {
        currentElement.after(draggingElement);
      } else {
        currentElement.before(draggingElement);
      }
    }
  });

  details.addEventListener('dragenter', (e) => {
    const currentElement = e.target.closest('details');
    if (currentElement) {
      currentElement.querySelector('summary').classList.add('dropzone');
    }
  });

  details.addEventListener('dragleave', (e) => {
    const currentElement = e.target.closest('details');
    if (currentElement) {
      currentElement.querySelector('summary').classList.remove('dropzone');
    }
  });
}

function updatePositions() {
  const detailsElements = document.querySelectorAll('details');
  detailsElements.forEach((details, newIndex) => {
    const summary = details.querySelector('summary');
    const positionInput = details.querySelector('input[name*="[position]"]');
    summary.textContent = `${newIndex + 1} - ${summary.textContent.split(' - ')[1]}`;
    positionInput.value = newIndex + 1;
  });
}
