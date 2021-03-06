/*
 * %kadu copyright begin%
 * Copyright 2012, 2013, 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#ifndef ERROR_SEVERITY_H
#define ERROR_SEVERITY_H

/**
 * @addtogroup Misc
 * @{
 */

/**
 * @enum ErrorSeverity
 * @short Error severity enum.
 */
enum ErrorSeverity
{
    /**
     * @short No error - everything is fine.
     */
    NoError,

    /**
     * @short Low severity error - can be ignored.
     */
    ErrorLow,

    /**
     * @short High severity error - should not be ignored.
     */
    ErrorHigh
};

/**
 * @}
 */

#endif   // ERROR_SEVERITY_H
