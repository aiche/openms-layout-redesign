// -*- mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2008 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
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

#ifndef OPENMS_FORMAT_MZMLFILE_H
#define OPENMS_FORMAT_MZMLFILE_H

#include <OpenMS/FORMAT/XMLFile.h>
#include <OpenMS/FORMAT/HANDLERS/MzMLHandler.h>
#include <OpenMS/FORMAT/PeakFileOptions.h>
#include <OpenMS/CONCEPT/ProgressLogger.h>

namespace OpenMS
{
	/**
		@brief File adapter for MzML files
		
		This implementation does currently not support the whole functionality of MzML.
		Some minor features are still missing:
  	  - chromatograms
  	  - zlib compression of base64 data
  	  - base64 integer data
  	  - base64 16 bit data
		
		If a critical error occurs due to the missing functionality, Exception::NotImplemented is thrown.
		
		@todo Fix open ussues of the format, add to automatic semantic validation in tests (Marc)
		@todo Implement Base64 integers, 16 bit, zlib support and chromatograms (Hiwi)
		
		@ingroup FileIO
	*/
	class MzMLFile 
		:	public Internal::XMLFile,
			public ProgressLogger
	{
		public:
			///Default constructor
			MzMLFile();
			///Destructor
			~MzMLFile();
			
      /// Mutable access to the options for loading/storing 
      PeakFileOptions& getOptions();

      /// Non-mutable access to the options for loading/storing 
      const PeakFileOptions& getOptions() const;

			/**
				@brief Loads a map from a MzML file.

				@p map has to be a MSExperiment or have the same interface.

				@exception Exception::FileNotFound is thrown if the file could not be opened
				@exception Exception::ParseError is thrown if an error occurs during parsing
			*/
			template <typename MapType>
			void load(const String& filename, MapType& map)
			{
				map.reset();
				
				Internal::MzMLHandler<MapType> handler(map,filename,schema_version_,*this);
				handler.setOptions(options_);
				parse_(filename, &handler);
			}

			/**
				@brief Stores a map in a MzML file.

				@p map has to be a MSExperiment or have the same interface.

				@exception Exception::UnableToCreateFile is thrown if the file could not be created
			*/
			template <typename MapType>
			void store(const String& filename, const MapType& map) const
			{
				Internal::MzMLHandler<MapType> handler(map,filename,schema_version_,*this);
				handler.setOptions(options_);
				save_(filename, &handler);
			}

			/**
				@brief Checks if a file validates against the XML schema.
				
		  	@exception Exception::FileNotFound is thrown if the file cannot be found.
				@exception Exception::NotImplemented is thrown if there is no schema available for the file type.
			*/
			bool isValid(const String& filename);

			/**
				@brief Checks if a file is valid with respect to the mapping file and the controlled vocabulary.
				
				@param errors Errors during the validation are returned in this output parameter.
				@param warnings Warnings during the validation are returned in this output parameter.
				
				@exception Exception::FileNotFound is thrown if the file could not be opened
			*/
			bool isSemanticallyValid(const String& filename, StringList& errors, StringList& warnings);

		private:
			
			/// Options for loading / storing
			PeakFileOptions options_;
			
			/// Location of indexed mzML schema
			String indexed_schema_location_;
	};

} // namespace OpenMS

#endif // OPENMS_FOMAT_MZMLFILE_H
