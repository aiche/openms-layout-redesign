// -*- mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2009 -- Oliver Kohlbacher, Knut Reinert
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
// $Maintainer: Andreas Bertsch $
// $Authors: Andreas Bertsch $
// --------------------------------------------------------------------------

#ifndef OPENMS_CHEMISTRY_MODIFICATIONSDB_H
#define OPENMS_CHEMISTRY_MODIFICATIONSDB_H

#include <OpenMS/DATASTRUCTURES/Map.h>
#include <OpenMS/CHEMISTRY/ResidueModification.h>

#include <set>

namespace OpenMS
{
	// forward declarations
	class ResidueModification;
	class Residue;

	/** @ingroup Chemistry
	
			@brief database which holds all residue modifications from PSI-MOD
			
			This singleton class serves as a storage of the available modifications
			represented by the PSI-MOD ontology. Each of the modifications is identified by
			their unique PSI-MOD identifier, e.g. MOD:01214. This identifier is also used
			in AASequence to specifiy modifications. Also residues can be modified by setting
			a modification using the unique identifier. Some modifications also have synonyms 
			which are also unique.

	*/
	class OPENMS_DLLAPI ModificationsDB
	{					
		public:

			inline static ModificationsDB* getInstance()
      {
        static ModificationsDB* db_ = 0;
        if (db_ == 0)
        {
          db_ = new ModificationsDB;
        }
        return db_;
      }

			/// returns the number of modifications read from the unimod.xml file
			Size getNumberOfModifications() const;

			/// returns the modification with the given index 
			const ResidueModification& getModification(Size index) const;

			/// returns all modifications which have the given name as synonym
			std::set<String> searchModifications(const String& name) const;

			/// returns all modification which have the given name as synonym and the given origin
			std::set<String> searchModifications(const String& name, const String& origin) const;
			
			/** @brief returns the modifications of the given name

					This can either be the PSI-MOD identifier or every other unique
					identifier which can be found in the PSI-MOD definitions file.

					To search for more than one modification searchModifications can be used!
					
					@exception ElementNotFound is thrown if no or more than one element is found
			*/
			const ResidueModification& getModification(const String& name) const;

			
			/// returns the modification with the given name and given residue 
			const ResidueModification& getModification(const String& residue_name, const String& mod_name) const;

			/// returns the index of the modification in the mods_ vector; a unique name must be given
			Size findModificationIndex(const String& mod_name) const;

			/// query the modifications DB to get the modifications with mass, without any specific origin
			void getModificationsByDiffMonoMass(std::vector<String>& mods, double mass, double error = 0.0);
			
			/// query the modifications DB to get modifications with the given mass at the given residue
			void getModificationsByDiffMonoMass(std::vector<String>& mods, const String& residue, double mass, double error = 0.0);

			/// adds modifications from a given file in OBO format
			void readFromOBOFile(const String& filename);

			/// adds modifications from a given file in Unimod XML format
			void readFromUnimodXMLFile(const String& filename);
			
		protected:

			/// stores the modifications
			std::vector<ResidueModification*> mods_;

			/// stores the mappings of (unique) names to the modifications
			Map<String, std::set<const ResidueModification*>  > modification_names_;
			
			
		private:

			/** @name Constructors and Destructors
      */
      //@{
      /// default constructor
      ModificationsDB();

      ///copy constructor
      ModificationsDB(const ModificationsDB& residue_db);

      /// destructor
      virtual ~ModificationsDB();
      //@}

      /** @name Assignment
      */
      //@{
      /// assignment operator
      ModificationsDB& operator = (const ModificationsDB& aa);
      //@}		
	};
}
#endif
