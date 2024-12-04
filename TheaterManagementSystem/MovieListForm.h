#include "SeatingGridForm.h"
#pragma once

namespace TheaterManagementSystem {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class MovieListForm : public Form // ref class for winforms and .net features
    {
    public:
        MovieListForm(void) // constructor
        {
            InitializeComponent();
            InitializeSeatData();
        }

    protected:
        ~MovieListForm() // destructor
        {
            // check components container and clear it
            if (components) 
            {
                delete components;
            }
        }

    private:
        System::ComponentModel::Container^ components; // container called 'components' for winforms elements

        // winforms ui elements
        Label^ headerLabel;
        DataGridView^ movieGrid;
        Button^ selectMovieButton;
        Button^ resetAllButton;

        array<String^>^ movies; // array of string objects for movie titles
        array<array<bool>^>^ seats; // array of arrays of bool objects, e.g. seats[0][5] = bool for availability of 6th seat in the 1st movie

/* -------------------------------------------------------------------------------------------------------------------- */
#pragma region Windows Form Designer generated code (InitializeComponent() and item handlers)
        void InitializeComponent(void)
        {
            this->Text = L"Movie List";
            this->Size = System::Drawing::Size(900, 600);
            this->BackColor = Color::White;

            this->headerLabel = gcnew Label();
            this->headerLabel->Text = L"Select a Movie:";
            this->headerLabel->Font = gcnew Drawing::Font(L"Segoe UI", 18, FontStyle::Bold);
            this->headerLabel->Location = Point(20, 20);
            this->headerLabel->AutoSize = true;

            this->movieGrid = gcnew DataGridView();
            this->movieGrid->Location = Point(20, 80);
            this->movieGrid->Size = System::Drawing::Size(840, 400);
            this->movieGrid->ReadOnly = true;
            this->movieGrid->AllowUserToAddRows = false;
            this->movieGrid->RowHeadersVisible = false;
            this->movieGrid->ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->movieGrid->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            this->movieGrid->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            this->movieGrid->Columns->Add(L"TheaterNumber", L"Theater Number");
            this->movieGrid->Columns->Add(L"Title", L"Title");
            this->movieGrid->Columns->Add(L"SeatsAvailable", L"Seats Available");

            this->selectMovieButton = gcnew Button();
            this->selectMovieButton->Text = L"Select Movie";
            this->selectMovieButton->Font = gcnew Drawing::Font(L"Segoe UI", 14);
            this->selectMovieButton->Size = System::Drawing::Size(200, 50);
            this->selectMovieButton->Location = Point(20, 500);

            this->resetAllButton = gcnew Button();
            this->resetAllButton->Text = L"Reset All Seats";
            this->resetAllButton->Font = gcnew Drawing::Font(L"Segoe UI", 14);
            this->resetAllButton->Size = System::Drawing::Size(200, 50);
            this->resetAllButton->Location = Point(240, 500);

            this->Controls->Add(this->headerLabel);
            this->Controls->Add(this->movieGrid);
            this->Controls->Add(this->selectMovieButton);
            this->Controls->Add(this->resetAllButton);

            this->movieGrid->CellDoubleClick += gcnew DataGridViewCellEventHandler(this, &MovieListForm::OnRowDoubleClicked);
            this->selectMovieButton->Click += gcnew EventHandler(this, &MovieListForm::OnSelectMovieClicked);
            this->resetAllButton->Click += gcnew EventHandler(this, &MovieListForm::OnResetAllSeatsClicked);
        }

        void OnRowDoubleClicked(Object^ sender, DataGridViewCellEventArgs^ e)
        {
            if (e->RowIndex >= 0) {
                SelectMovie(e->RowIndex);
            }
        }

        void OnSelectMovieClicked(Object^ sender, EventArgs^ e)
        {
            if (this->movieGrid->SelectedRows->Count > 0) {
                SelectMovie(this->movieGrid->SelectedRows[0]->Index);
            }
            else {
                MessageBox::Show(L"Please select a movie first.", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void OnResetAllSeatsClicked(Object^ sender, EventArgs^ e)
        {
            if (MessageBox::Show(L"Reset all seats for all movies?", L"Confirm", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
                for (int i = 0; i < seats->Length; ++i) {
                    for (int j = 0; j < 100; ++j) {
                        seats[i][j] = true;
                    }
                }
                UpdateMovieGrid();
            }
        }
#pragma endregion 
/* -------------------------------------------------------------------------------------------------------------------- */
#pragma region Movie list titles and seats (InitializeSeatData() and UpdateMovieGrid())
        void InitializeSeatData()
        {
            // create movie titles array
            movies = gcnew array<String^>{
                L"Spider-Man: No Way Home",
                    L"Inside Out 2",
                    L"Barbie",
                    L"John Wick",
                    L"Dune",
                    L"Top Gun: Maverick",
                    L"Super Mario Bros. Movie"
            };

            // new array of arrays of bools, one for all 7 movies
            seats = gcnew array<array<bool>^>(7);

            // initialize seat availability for each movie
            for (int i = 0; i < 7; ++i) {
                // new array of 100 bools for the current movie's seats
                seats[i] = gcnew array<bool>(100);

                // set all seats for the movie to available/true
                for (int j = 0; j < 100; ++j) {
                    seats[i][j] = true;
                }

                // add a new row to the movie grid with the movie's details
                //  i + 1: theater number,  movies[i]: movie title,  "100/100": all seats are available initially
                this->movieGrid->Rows->Add(i + 1, movies[i], L"100/100");
            }
        }

        void UpdateMovieGrid()
        {
            for (int i = 0; i < 7; ++i) {
                String^ movie = movies[i]; // get the movie name from the movies array

                // get the number of available seats for this movie
                int availableSeats = 0;
                for each (bool isAvailable in seats[i]) {
                    if (isAvailable) ++availableSeats; // increment for each available seat
                }

                // update the "Seats Available" column
                this->movieGrid->Rows[i]->Cells[2]->Value = String::Format(L"{0}/100", availableSeats);
            }
        }
#pragma endregion
/* -------------------------------------------------------------------------------------------------------------------- */
#pragma region Launch seat grid for movie (SelectMovie)
        void SelectMovie(int index) // open the seating grid for a movie (next menu)
        {
            // get the title of the selected movie directly from the array
            String^ movie = movies[index];

            // create and display a new SeatingGridForm for the selected movie
            // pass the movie title and seat data to the form
            SeatingGridForm^ seatingGridForm = gcnew SeatingGridForm(movie, seats[index]);
            seatingGridForm->ShowDialog(); // open the winform as a new window

            UpdateMovieGrid(); // update seat availabilities after returning from grid menu
        }
#pragma endregion
/* -------------------------------------------------------------------------------------------------------------------- */
    };
}