document.addEventListener('DOMContentLoaded', function () {
    const components = document.querySelectorAll('.component');
    const canvas = document.querySelector('.canvas');
    let draggedComponent = null;

    components.forEach(component => {
        component.setAttribute('draggable', 'true');

        component.addEventListener('dragstart', function (e) {
            e.dataTransfer.setData('text/plain', component.textContent);
            draggedComponent = component;

            if (canvas.contains(draggedComponent)) {
                const hiddenPlaceholder = canvas.querySelector('.placeholder.hidden');
                if (hiddenPlaceholder) {
                    hiddenPlaceholder.classList.remove('hidden');
                }
            }

            setTimeout(() => {
                component.style.opacity = '0.5';
            }, 0);
        });

        component.addEventListener('dragend', function () {
            if (draggedComponent) {
                draggedComponent.style.opacity = '1';
            }
        });
    });

    canvas.addEventListener('dragover', function (e) {
        e.preventDefault();
    });

    canvas.addEventListener('drop', function (e) {
        e.preventDefault();

        const placeholders = canvas.querySelectorAll('.placeholder:not(.hidden)');
        let closest = null;
        let shortestDistance = Infinity;

        placeholders.forEach(placeholder => {
            const rect = placeholder.getBoundingClientRect();
            const dx = e.clientX - (rect.left + rect.width / 2);
            const dy = e.clientY - (rect.top + rect.height / 2);
            const distance = dx * dx + dy * dy;

            if (distance < shortestDistance) {
                shortestDistance = distance;
                closest = placeholder;
            }
        });

        if (closest && draggedComponent) {
            canvas.insertBefore(draggedComponent, closest);
            closest.classList.add('hidden');
        }
    });
});
