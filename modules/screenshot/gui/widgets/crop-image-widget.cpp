/*
 * %kadu copyright begin%
 * Copyright 2010 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * %kadu copyright end%
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QResizeEvent>

#include "gui/graphics-items/selection-frame-item.h"

#include "crop-image-widget.h"

CropImageWidget::CropImageWidget(QWidget *parent) :
		QGraphicsView(parent)
{
	setContentsMargins(0, 0, 0, 0);
	setFrameShape(NoFrame);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QGraphicsScene *graphicsScene = new QGraphicsScene(this);
	setScene(graphicsScene);

	PixmapItem = new QGraphicsPixmapItem();
	PixmapItem->setX(0);
	PixmapItem->setY(0);

	scene()->addItem(PixmapItem);

	SelectionFrame = new SelectionFrameItem();
	SelectionFrame->setPos(0, 0);
	SelectionFrame->setSize(size());

	scene()->addItem(SelectionFrame);
}

CropImageWidget::~CropImageWidget()
{
}

void CropImageWidget::resizeEvent(QResizeEvent *event)
{
	SelectionFrame->setSize(event->size());
    QGraphicsView::resizeEvent(event);
}

void CropImageWidget::setPixmap(QPixmap pixmap)
{
	PixmapItem->setPixmap(pixmap);
}

void CropImageWidget::setCropRect(QRect cropRect)
{
	SelectionFrame->setSelection(cropRect.normalized());
	scene()->update(scene()->sceneRect());
}
