#ifndef AME20PARSER_H
#define AME20PARSER_H
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <regex>
#include <iterator>
class MassEval2020{
    private:
    // Sam's addition  - fetching isotope name given N, Z values
    std::map<std::pair<int, int>, std::string> IsotopeName;
    // Sam's addition - fetching N, Z values given isotope name (Geant format (A-string))
    std::map<std::string, std::pair<int, int>> IsotopeNZGeant;
    std::map<std::pair<int,int>, long double> MassTab; // look up isotope mass based on <N,Z> pair
    std::map<std::string, std::pair<int,int>> Isotope; // loop up isotope <N,Z> from isotope name XxA
    std::map<std::string, long double> IsotopeMass; // loop up isotoe mass from isotope name XxA

    const long double uToMeV = 931.494102423;

    public:
    /**
     * @brief Create isotope/mass table from an AME2020 data file
     * @param ame20path string filepath to the ame2020 file (untruncated)
     * @param diag enable diagnostic printing to check that the mass file is parsed appropriately
    */
    MassEval2020(std::string ame20path="mass_1.mas20", bool diag=false){
        MakeTables(ame20path, diag);
    }
    ~MassEval2020(){} // Trivial destructor
    /**
     * @brief Retrieve the mass of an isotope (in u by default)
     * @param isoname string for the name of the isotope of interest (e.g. Co57)
     * @param inMeV return the isotope mass in MeV/c^2 instead of u
     * @return isotope mass (in u by default, or use @param inMeV = true to get in MeV/c^2)
    */
    long double Mass(std::string isoname, bool inMeV=false){
        auto m = IsotopeMass[isoname];
        if (inMeV){
            m = m*uToMeV; 
        }
        return m;
    }
    /**
     * @brief Retrieve the mass of an isotope (in u by default)
     * @param n (int) neutron number of isotope
     * @param z (int) proton number of isotope
     * @param inMeV return the isotope mass in MeV/c^2 instead of u
     * @return isotope mass (in u by default, or use @param inMeV = true to get in MeV/c^2)
    */
    long double Mass(int n, int z, bool inMeV=false){
        auto m = MassTab[std::make_pair(n,z)];
        if (inMeV){
            m = m*uToMeV; 
        }
        return m;
    }
    /**
     * @brief Retrieve the mass of an isotope (in u by default)
     * @param nz std::pair<int,int> (neutron, proton) numbers (in an stl pair) of isotope
     * @param inMeV return the isotope mass in MeV/c^2 instead of u
     * @return isotope mass (in u by default, or use @param inMeV = true to get in MeV/c^2)
    */
    long double Mass(std::pair<int,int> nz, bool inMeV=false){
        auto m = MassTab[nz];
        if (inMeV){
            m = m*uToMeV; 
        }
        return m;
    }
    /**
     * @brief Get the N, Z of an isotope
     * @param isoname name of the isotope (e.g. He3)
     * @return std::pair<int,int> (N,Z)
    */
    std::pair<int,int> NZ(std::string isoname){
        return Isotope[isoname];
    }

    // Sam's addition :)
    // Given the N and Z values for an isototpe, return the name in a Geant acceptable format (A-string) (ex. 3He)
    // The pair NZ is formatted (N, Z)
    std::string GetIsotopeName(std::pair<int, int> NZ){
	return IsotopeName[NZ];
    }

    // Sam's addition - 7/25/24
    // Fetch the N/Z values given an isotope name in Geant form (A-string) (ex. 3He, 40Ca)
    std::pair<int, int> GetIsotopeNZGeant(std::string isotopeName){
	   return IsotopeNZGeant[isotopeName]; 
    }

    /**
     * @brief Get the Q-value (in MeV) of a reaction between specified isotopes
     * @param reactants std::vector<std::<int,int>> list of reactant <N,Z> pairs
     * @param products std::vector<std::<int,int>> list of product <N,Z> pairs
     * @return Q-value of reaction in MeV
    */
    long double Qval(std::vector<std::pair<int,int>> reactants, std::vector<std::pair<int,int>> products){
        long double qval = 0;
        for (auto rx : reactants){
            qval += Mass(rx, true);
        }
        for (auto px : products){
            qval -= Mass(px, true);
        }
        return qval;
    }
    
    /**
     * @brief Get the Q-value (in MeV) of a reaction between specified isotopes
     * @param reactants std::vector<std::string> list of reactant <N,Z> pairs
     * @param products std::vector<std::string> list of product <N,Z> pairs
     * @return Q-value of reaction in MeV
    */
    long double Qval(std::vector<std::string> reactants, std::vector<std::string> products){
        long double qval = 0;
        for (auto rx : reactants){
            qval += Mass(rx, true);
        }
        for (auto px : products){
            qval -= Mass(px, true);
        }
        return qval;
    }
    
    /**
     * @brief Get the Q-value (in MeV) of a reaction between specified isotopes (calculate recoil nucleus based off exit channel)
     * @param projectile std::pair<int,int> beam isotope <N,Z>
     * @param target std::pair<int,int> target isotope <N,Z>
     * @param exit_channel std::vector<std::<int,int>> list of product <N,Z> pairs for exit channel
     * @return Q-value of reaction in MeV
    */
    long double Qval(std::pair<int,int> projectile, std::pair<int,int> target, std::vector<std::pair<int,int>> exit_channel){
        long double qval = Mass(projectile, true) + Mass(target, true);
        int nn = projectile.first + target.first;
        int nz = projectile.second + target.second;
        for (auto px : exit_channel){
            // note that we need to balance the nucleon numbers in the reaction
            // so keep track of the N, Z we account for in the exit channel
            // and use this to determine the recoiling nucleus isotope
            nn -= px.first;
            nz -= px.second;
            
            qval -= Mass(px, true);
        }
        qval -= Mass(std::make_pair(nn,nz), true);
        return qval;
    }
    /**
     * @brief Get the Q-value (in MeV) of a reaction between specified isotopes (calculate recoil nucleus based off exit channel)
     * @param projectile std::string beam isotope <N,Z>
     * @param target std::string target isotope <N,Z>
     * @param exit_channel std::vector<std::string> list of product isotope names for exit channel
     * @return Q-value of reaction in MeV
    */
    long double Qval(std::string projectile, std::string target, std::vector<std::string> exit_channel){
        // this function overload is really a wrapper around a version which uses <N,Z> pairs to search the tables and calculate the Q-val
        // let's create a vector of exit channel isotope names
        std::vector<std::pair<int,int>> exit_pair;
        for (auto px : exit_channel){
            exit_pair.push_back(NZ(px));
        }
        return Qval(NZ(projectile), NZ(target), exit_pair);
    }

    /**
     * @brief Fill object tables to reference isotope mass values from ame20 based on <N,Z> upon instantiation
     * @param ame20path string filepath to the ame2020 file (untruncated)
     * @param diag enable diagnostic printing to check that the mass file is parsed appropriately
    */
    void MakeTables(std::string ame20path="mass_1.mas20", bool diag=false){
        std::fstream ame;
        ame.open(ame20path, std::ios::in);
        if (ame.is_open()){
            std::string line;
            // skip the header lines
            // note that I am also skipping the neutron line here...
            // not needed for us and it could get confused with Nitorgen
            for (int i = 0; i < 37; i++){
                getline(ame, line);
            }
            std::string sn, sz, sm, el;
            int n;
            int z;
            std::pair<int,int> NZ;
            int a;
            long double m;
            std::string sa; // string A
            std::string isoname; // Isotope name e.g. He3
            // diagnostic printing vars
            std::string colid;
            bool skipNextChar = false;
            std::cout.precision(15);

            while(getline(ame, line)){
                if (diag){
                    colid = "";
                    for (int i = 0; i < (int) line.length(); i++){
                        if (i%10 == 0){
                            colid+=std::to_string(i/10);
                            if (i/10 >=10){
                                skipNextChar = true;
                            }
                        }
                        else if (!skipNextChar){
                            colid+=".";
                        }
                        else {
                            skipNextChar = false;
                        }
                    }
                    std::cout << "Raw line; no parsing" << std::endl;
                    std::cout << colid << std::endl;
                    std::cout << line << std::endl;
                }
                sn = line.substr(6,3);
                sz = line.substr(11,3);
                el = line.substr(20,2);
                sm = line.substr(106,15);
                if (diag){
                    std::cout << "Parsing by column only to separate vars:" << std::endl;
                    std::cout << sn << "," << sz << "," << el << "," << sm << std::endl;
                }

                sn.erase(remove(sn.begin(),sn.end(),' '),sn.end());
                sz.erase(remove(sz.begin(),sz.end(),' '),sz.end());
                sm.erase(remove(sm.begin(),sm.end(),' '),sm.end());
                el.erase(remove(el.begin(),el.end(),' '),el.end());

                //std::regex_replace(sm, std::regex(R"([\D])"), "");
                sm.erase(remove(sm.begin(),sm.end(),'#'),sm.end());
                if (diag){
                    std::cout << "Data cleaning step; removing stray characters:" << std::endl;
                    std::cout << sn << "," << sz << "," << el << "," << sm << std::endl;
                }

                n = std::stoi(sn);
                z = std::stoi(sz);
                a = n + z;
                m = std::stod(sm)/1e6;
                if (diag){
                    std::cout << "Final conversion from strings to numeric types" << std::endl;
                    std::cout << el << a << ": N = " << n << ", Z = " << z << ", mass = " << m << "u" << std::endl;
                    getchar();
                }
                // Now let's fill the maps
                NZ.first = n;
                NZ.second = z;
                sa = std::to_string(a);
                // format 1: first letter capitalized, second lowercase if it exists (e.g. He4, H3)
                isoname = el + sa;
                Isotope[isoname] = NZ;
		
		// Sam's addition
		std::string isonameGeant = sa + el; 
		IsotopeName[NZ] = isonameGeant;

		// Sam's addition - 7/25/24
		IsotopeNZGeant[isonameGeant] = NZ;
		// end addition

                // We can also flip the other way (e.g. 4He)
		/*
                isoname = sa + el;
                Isotope[isoname] = NZ;
                // format 2: both lettters lowercase (e.g. he4, h3)
                for (long unsigned int il = 0; il < el.length(); il++){
                    el[il] = std::tolower(el[il]);
                }
                isoname = el + sa;
                Isotope[isoname] = NZ;
                isoname = sa + el;
                Isotope[isoname] = NZ;
		*/

                // format 3: both letter capitalized (e.g. HE4)
                // note: we only need to do this if there are two letterse in the element symbol
		/*
                if (el.length() > 1){
                    for (long unsigned int il = 0; il < el.length(); il++){
                        el[il] = std::toupper(el[il]);
                    }
                    isoname = el + sa;
                    Isotope[isoname] = NZ;
                    
                    isoname = sa + el;
                    Isotope[isoname] = NZ;
		    std::cout << "parser: " << isoname << std::endl;
                }
		*/
                
                // finally, fill the mass table
                MassTab[NZ] = m;
            }
            // SPECIAL CASES (Isotope Table)
            // We want to fill in (p, d, t, a) -> (H1, H2, H3, He4) so we can make use of shorthand notation
            // Note that we can do this with the map because we won't add the A to the end of the isotope name
            // (this would prevent confusion between p for proton and p32 for phosphorus isotopes)
            Isotope["p"] = std::make_pair(0,1);
            Isotope["d"] = std::make_pair(1,1);
            Isotope["t"] = std::make_pair(2,1);
            Isotope["a"] = std::make_pair(2,2);

            // Finally, let us fill the IsotopeMass mapping based on the MassTab and use the Isotope map for keys
            for (const auto &item : Isotope){
                IsotopeMass[item.first] = MassTab[item.second];
            }
        }
        else{
            std::cerr << "Error: cannot find or open your ame20 file.  Path provided = " << ame20path << std::endl;
            exit(1);
        }
    }
};

#endif
