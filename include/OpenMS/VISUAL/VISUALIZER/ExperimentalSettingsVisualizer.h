// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2005 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free ExperimentalSettings Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Marc Sturm $
// --------------------------------------------------------------------------
 
#ifndef OPENMS_VISUAL_VISUALIZER_EXPERIMENTALSETTINGSVISUALIZER_H
#define OPENMS_VISUAL_VISUALIZER_EXPERIMENTALSETTINGSVISUALIZER_H

//OpenMS
#include <OpenMS/METADATA/ExperimentalSettings.h>
#include <OpenMS/VISUAL/VISUALIZER/BaseVisualizer.h>

//QT
class QLineEdit;
class QComboBox;

namespace OpenMS 
{

	class MSMetaDataExplorer;
	
	/**
	@brief Class that displays all meta information for ExperimentalSettings objects
	
	This class provides all functionality to view the meta information of an object of type ExperimentalSettings.
	*/
	
	class ExperimentalSettingsVisualizer : public BaseVisualizer
	{
		Q_OBJECT

	public: 
	  /// Default constructor
		ExperimentalSettingsVisualizer(bool editable= FALSE, QWidget *parent =0);
		
		/// Loads the meta data from the object to the viewer.
		void load(ExperimentalSettings &s);
	  
	private slots:
		 /// Saves the changes made to the meta data into the object.
		void store_();
		/// Deletes all changes made in the viewer and restores the original meta data.
		void reject_();

	private:  
		/// Pointer to current object to keep track of the actual object
		ExperimentalSettings *ptr_;
		/// Copy of current object for restoring the original values
		ExperimentalSettings  tempexperimentalsettings_;
		/// Fills the comboboxes with current values
		void update_();
	  
		/// The type of this experiment
   	QComboBox *experimentalsettings_type_;
		/// The date of this experiment
		QLineEdit *experimentalsettings_date_;		
		/// The comment to this experiment
		QTextEdit *experimentalsettings_comment_;
	};
}
#endif
